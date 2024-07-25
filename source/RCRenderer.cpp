/*
 * Copyright (c) 2023~Now Margoo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * \file RCRenderer.cpp
 * \brief RC 引擎中对场景的渲染封装
 */

#include <include/RCRenderer.h>

#include <algorithm>

RCRenderer::RCRenderer(RCRenderTarget *RenderTarget, RCCamera *Camera, RCScene *Scene)
    : _renderTarget(RenderTarget), _camera(Camera), _scene(Scene),
      _enableResolution(false) {
	if (_renderTarget == nullptr || _camera == nullptr || _scene == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCRenderer construction");
	}

	_renderTargetWidth  = _renderTarget->GetContext()->GetWidth();
	_renderTargetHeight = _renderTarget->GetContext()->GetHeight();

	_fogColorR = GetRValue(_scene->_fogColor);
	_fogColorG = GetGValue(_scene->_fogColor);
	_fogColorB = GetBValue(_scene->_fogColor);

	_contextResolution      = new RCContext(RenderTarget->_context->GetWidth() / 2, RenderTarget->_context->GetHeight() / 2);
	_resolutionRenderTarget = new RCRenderTarget(_contextResolution);
	_resolutionDC           = GetImageHDC(_contextResolution->_context);
	_renderTargetDC         = GetImageHDC(RenderTarget->GetImage());

	_resolutionWidth  = _renderTargetWidth / 2;
	_resolutionHeight = _renderTargetHeight / 2;

	PitchMax = _renderTargetHeight / 4.f;

	_renderTargetDC = GetImageHDC(_renderTarget->GetImage());
	_resolutionDC   = GetImageHDC(_resolutionRenderTarget->GetImage());

#ifdef _RC_RENDER_DEBUGER_
	// 初始化调试器字体
	gettextstyle(&_debuggerFont);
	_tcscpy_s(_debuggerFont.lfFaceName, _T("Times New Roman"));
	_debuggerFont.lfHeight  = 16;
	_debuggerFont.lfQuality = PROOF_QUALITY;

	// 初始化调试器计时器
	_oldClock = clock();

	setbkmode(TRANSPARENT);
#endif
}
void RCRenderer::EnableSuperResolution(const bool &Status) {
	_enableResolution = Status;
	if (_enableResolution) {
		_renderTargetWidth  = _resolutionWidth;
		_renderTargetHeight = _resolutionHeight;
	}
	else {
		_renderTargetWidth  = _renderTarget->_context->GetWidth();
		_renderTargetHeight = _renderTarget->_context->GetHeight();
	}

	PitchMax = _renderTargetHeight / 4.f;
}
void RCRenderer::SetScene(RCScene *Scene) {
	if (Scene == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCRenderer.SetScene");
	}
	if (!Scene->CheckValid()) {
		throw RCInvalidParameterException("Invalid RCScene object", "RCRenderer construction");
	}

	_scene     = Scene;
	_fogColorR = GetRValue(_scene->_fogColor);
	_fogColorG = GetGValue(_scene->_fogColor);
	_fogColorB = GetBValue(_scene->_fogColor);
}
float RCRenderer::Render() {
	_renderTarget->Clear();

	time_t frameStart = clock();

	const auto pitch       = _camera->_pitch * PitchMax;
	const auto fogConstant = (_scene->_map->GetWidth() + _scene->_map->GetHeight()) / 2;
	vecmath::Vector<float> rayRightDirection = _camera->Direction - _camera->Plane;
	vecmath::Vector<float> rayLeftDirection  = _camera->Direction + _camera->Plane;

	float cameraZFloor = 0.5f * _renderTargetHeight + _camera->Z;
	float cameraZCeiling = 0.5f * _renderTargetHeight - _camera->Z;

	// 如果启用天空盒，则渲染天空盒
	if (_scene->_enableSkybox) {
		RenderSkyBox(_renderTargetWidth, _renderTargetHeight, pitch, fogConstant, rayRightDirection, rayLeftDirection, 0);
	} else {
		// 否则渲染天花板
		RenderCeiling(_renderTargetWidth, _renderTargetHeight, pitch, fogConstant, rayRightDirection, rayLeftDirection, cameraZCeiling, 0);
	}
	// 渲染地板
	RenderFloor(_renderTargetWidth, _renderTargetHeight, pitch, fogConstant, rayRightDirection, rayLeftDirection, cameraZFloor, 0);
	// 渲染墙体
	RayCasting(_renderTargetWidth, _renderTargetHeight, pitch, fogConstant, rayRightDirection, rayLeftDirection, 0);

	if (_enableResolution) {
		StretchBlt(_renderTargetDC, 0, 0, _renderTargetWidth * 2, _renderTargetHeight * 2, _resolutionDC,
		           0, 0, _renderTargetWidth, _renderTargetHeight, SRCCOPY);
	}

	auto logicalFrame = static_cast<float>(clock() - frameStart) / 1000.f;

#ifdef _RC_RENDER_DEBUGER_
	OutDebugText();

	++_fpsTemp;
	if (clock() - _oldClock >= 1000) {
		_fpsCount = _fpsTemp;
		_fpsTemp  = 0;
		_oldClock = clock();
	}

	outtextxy(50, 150, std::format(_T("Logical frame : {}"), logicalFrame).c_str());
#endif

	return logicalFrame < 0.001f ? 0.001f : logicalFrame;
}
void RCRenderer::RenderFloor(const int &Width, const int &Height, const float &Pitch, const int& FogConstant,
                             const vecmath::Vector<float>& RayRightDirection, const vecmath::Vector<float>& RayLeftDirection,
                             const float& CameraZ, const int &Start) {
	const auto textureWidth  = _scene->_floorTexture->_context->GetWidth();
	const auto textureHeight = _scene->_floorTexture->_context->GetHeight();

	auto bufferPointer = _enableResolution ? _resolutionRenderTarget->_backBuffer : _renderTarget->_backBuffer;

	// 渲染地板
	for (int y = Height / 2 + Pitch + 1, relative = 1; y < Height; ++y, ++relative) {
		auto verticalPosition = y * Width;

		float floorDistance  = CameraZ / static_cast<float>(relative);

		vecmath::Vector<float> floorStep    = floorDistance * (RayLeftDirection - RayRightDirection) / static_cast<float>(Width);
		vecmath::Vector<float> realPosition = _camera->Position + floorDistance * RayRightDirection;

		float fog = 0;
		if (_scene->_enableFog) {
			// 越远烟雾越浓
			fog = floorDistance / static_cast<float>(FogConstant) * _scene->_fogLevel;
		}

		for (int x = 0; x < Width; ++x) {
			vecmath::Vector<int>    texture(textureWidth, textureHeight, 0);
			float cellX = static_cast<int>(realPosition.x);
			float cellY = static_cast<int>(realPosition.y);
			int textureX = static_cast<int>(texture.x * (realPosition.x - cellX)) & (textureWidth - 1);
			int textureY = static_cast<int>(texture.y * (realPosition.y - cellY)) & (textureHeight - 1);

			realPosition += floorStep;

			auto textureColor = _scene->_floorTexture->_buffer[textureWidth * textureY + textureX];
			// 如果启用了烟雾，则计算烟雾效果
			if (_scene->_enableFog) {
				if (fog >= 1) {
					textureColor = _scene->_fogColor;
				} else if (fog > 0) {
					auto r = GetRValue(textureColor) * (1 - fog) + _fogColorR * fog;
					auto g = GetGValue(textureColor) * (1 - fog) + _fogColorG * fog;
					auto b = GetBValue(textureColor) * (1 - fog) + _fogColorB * fog;

					textureColor = RGB(r, g, b);
				}
			}

			textureColor                                = (textureColor >> 1) & 8355711;
			bufferPointer[verticalPosition + x]   = textureColor;
		}
	}
}
void RCRenderer::RenderCeiling(const int &Width, const int &Height, const float &Pitch,
                               const int &FogConstant, const vecmath::Vector<float> &RayRightDirection,
                               const vecmath::Vector<float> &RayLeftDirection,
                               const float &CameraZ,
                               const int &Start) {
	const auto textureWidth  = _scene->_ceilingTexture->_context->GetWidth();
	const auto textureHeight = _scene->_ceilingTexture->_context->GetHeight();

	auto bufferPointer = _enableResolution ? _resolutionRenderTarget->_backBuffer : _renderTarget->_backBuffer;

	// 渲染地板
	for (int y = Height / 2 + Pitch + 1, relative = 1; y >= 0; --y, ++relative) {
		auto verticalPosition = y * Width;
		float floorDistance   = CameraZ / static_cast<float>(relative);

		vecmath::Vector<float> floorStep    = floorDistance * (RayLeftDirection - RayRightDirection) / static_cast<float>(Width);
		vecmath::Vector<float> realPosition = _camera->Position + floorDistance * RayRightDirection;

		float fog = 0;
		if (_scene->_enableFog) {
			// 越远烟雾越浓
			fog = floorDistance / static_cast<float>(FogConstant) * _scene->_fogLevel;
		}

		for (int x = 0; x < Width; ++x) {
			vecmath::Vector<int> texture(textureWidth, textureHeight, 0);
			float cellX  = static_cast<int>(realPosition.x) - realPosition.x;
			float cellY  = static_cast<int>(realPosition.y) - realPosition.y;
			int textureX = static_cast<int>(texture.x * cellX) & (textureWidth - 1);
			int textureY = static_cast<int>(texture.y * cellY) & (textureHeight - 1);

			realPosition += floorStep;

			auto textureColor = _scene->_ceilingTexture->_buffer[textureWidth * textureY + textureX];
			// 如果启用了烟雾，则计算烟雾效果
			if (_scene->_enableFog) {
				if (fog >= 1) {
					textureColor = _scene->_fogColor;
				} else if (fog > 0) {
					auto r = GetRValue(textureColor) * (1 - fog) + _fogColorR * fog;
					auto g = GetGValue(textureColor) * (1 - fog) + _fogColorG * fog;
					auto b = GetBValue(textureColor) * (1 - fog) + _fogColorB * fog;

					textureColor = RGB(r, g, b);
				}
			}

			// 使颜色略黑
			textureColor                                     = (textureColor >> 1) & 8355711;
			bufferPointer[verticalPosition + x] = textureColor;
		}
	}
}
void RCRenderer::RenderSkyBox(const int &Width, const int &Height, const float &Pitch,
                  const int& FogConstant, const vecmath::Vector<float>& RayRightDirection,
                  const vecmath::Vector<float>& RayLeftDirection, const int &Start) {
	auto bufferPointer = _enableResolution ? _resolutionRenderTarget->_backBuffer : _renderTarget->_backBuffer;
	auto skyboxTextureWidth  = _scene->_skyBoxTexture->_context->GetWidth();
	auto skyboxTextureHeight = _scene->_skyBoxTexture->_context->GetHeight();
	int textureXRight = static_cast<int>(-atan2(RayRightDirection.y, RayRightDirection.x) *
	                                            (static_cast<double>(skyboxTextureWidth) / (2 * pi)) * _scene->_skyboxRepeats);
	int textureXLeft = static_cast<int>(-atan2(RayLeftDirection.y, RayLeftDirection.x) *
	                                            (static_cast<double>(skyboxTextureWidth) / (2 * pi)) * _scene->_skyboxRepeats);
	int textureX             = 0;
	while (textureXLeft < textureXRight) {
		textureXLeft += skyboxTextureWidth;
	}
	while (textureXRight < 0) {
		textureXRight   += skyboxTextureWidth;
		textureXLeft    += skyboxTextureWidth;
	}

	int deltaTextureX = textureXLeft - textureXRight;
	int deltaY        = Height / 2 + Pitch;
	int deltaTextureY = skyboxTextureHeight * (Height / 2 + Pitch) / (Height / 2 + PitchMax) - 1;
	int textureYRight = skyboxTextureHeight - 1 - deltaTextureY;
	int relativeX     = 0;
	for (int x = Start; x < Width; ++x) {
		if (textureXRight >= skyboxTextureWidth) {
			textureX = textureXRight - skyboxTextureWidth;
		}
		else {
			textureX = textureXRight;
		}
		int textureY  = 0;
		int relativeY = 0;
		for (int y = 0; y < deltaY; ++y) {
			COLORREF color = _scene->_skyBoxTexture->_buffer[skyboxTextureWidth * textureY + textureX];
			bufferPointer[y * Width + x] = color;

			relativeY += deltaTextureY;
			while (relativeY > deltaY) {
				textureY    += 1;
				relativeY   -= deltaY;
			}
		}

		relativeX += deltaTextureX;
		while (relativeX > Width) {
			textureXRight   += 1;
			relativeX       -= Width;
		}
	}
}
void RCRenderer::RayCasting(const int &Width, const int &Height, const float &Pitch,
                            const int &FogConstant, const vecmath::Vector<float> &RayRightDirection,
                            const vecmath::Vector<float> &RayLeftDirection, const int &Start) {
	auto bufferPointer = _enableResolution ? _resolutionRenderTarget->_backBuffer : _renderTarget->_backBuffer;
	for (int x = Start; x < Width; ++x) {
		float cameraX = 2.f * static_cast<float>(x) / static_cast<float>(_renderTargetWidth) - 1;
		vecmath::Vector<float> rayDirection = _camera->Direction + _camera->Plane * cameraX;

		/**
		 * 击中物体的方向
		 */
		auto objects = _mapObjectMemoryPool.allocate(10);
		auto size = 0;
		float furtherDistance;
		{
			int mapX = static_cast<int>(_camera->Position.x);
			int mapY = static_cast<int>(_camera->Position.y);

			RCRender::MapObject object{};
			float sideDistanceX;
			float sideDistanceY;
			float deltaDistanceX = std::abs(1.f / rayDirection.x);
			float deltaDistanceY = std::abs(1.f / rayDirection.y);
			float perpDistance;
			float wallX;

			int stepX;
			int stepY;

			if (rayDirection.x < 0) {
				stepX         = -1;
				sideDistanceX = (_camera->Position.x - static_cast<float>(mapX)) * deltaDistanceX;
			} else {
				stepX         = 1;
				sideDistanceX = (static_cast<float>(mapX) + 1.f - _camera->Position.x) * deltaDistanceX;
			}
			if (rayDirection.y < 0) {
				stepY         = -1;
				sideDistanceY = (_camera->Position.y - static_cast<float>(mapY)) * deltaDistanceY;
			} else {
				stepY         = 1;
				sideDistanceY = (static_cast<float>(mapY) + 1.f - _camera->Position.y) * deltaDistanceY;
			}

			RCRender::HideSide hitSide;

			while (true) {
				if (sideDistanceX < sideDistanceY) {
					sideDistanceX += deltaDistanceX;
					mapX += stepX;
					hitSide = RCRender::HideSide::NS;
				} else {
					sideDistanceY += deltaDistanceY;
					mapY += stepY;
					hitSide = RCRender::HideSide::EW;
				}
				RCMapUnit mapUnit = _scene->_map->_mapArray[mapX + mapY * _scene->_map->_width];
				if (mapUnit.Type != RCMapUnitType::Air) {
					if (mapUnit.Type == RCMapUnitType::Wall) {
						if (hitSide == RCRender::HideSide::NS) {
							perpDistance = sideDistanceX - deltaDistanceX;
						} else {
							perpDistance = sideDistanceY - deltaDistanceY;
						}
					}
					if (mapUnit.Type == RCMapUnitType::Door || mapUnit.Type == RCMapUnitType::Glass || mapUnit.Type == RCMapUnitType::Strip) {
						if (hitSide == RCRender::HideSide::NS) {
							float distance = sideDistanceX - deltaDistanceX * 0.5f;
							if (sideDistanceY < distance) {
								continue;
							}
							perpDistance = distance;
						} else {
							float distance = sideDistanceY - deltaDistanceY * 0.5f;
							if (sideDistanceX < distance) {
								continue;
							}
							perpDistance = distance;
						}
					}
					if (mapUnit.Type == RCMapUnitType::DiagWallLeftRight ||
					    mapUnit.Type == RCMapUnitType::DiagWallRightLeft) {
						struct Intersect {
							float perpDistance;
							float wallX;
						};
						float k;
						float distance;
						if (mapUnit.Type == RCMapUnitType::DiagWallLeftRight) {
							k        = 1.f;
							distance = _camera->Position.x - mapX - _camera->Position.y + mapY;
							if (rayDirection.y != rayDirection.x) {
								perpDistance = (mapY + k * (_camera->Position.x - mapX) - _camera->Position.y) / (rayDirection.y - k * rayDirection.x);
								wallX        = (_camera->Position.x + rayDirection.x * perpDistance - mapX);
							} else {
								wallX        = -1;
								perpDistance = -1;
							}
						}
						else {
							k        = -1.f;
							distance = mapX - _camera->Position.x - _camera->Position.y + mapY + 1;
							if (rayDirection.y != rayDirection.x) {
								perpDistance = (mapY + 1.f + k * (_camera->Position.x - mapX) - _camera->Position.y) / (rayDirection.y - k * rayDirection.x);
								wallX        = _camera->Position.x + rayDirection.x * perpDistance - mapX;
							} else {
								wallX        = -1;
								perpDistance = -1;
							}
						}

						if (wallX < 0.f || wallX >= 1.f) {
							continue;
						}

						if (distance < 0) {
							wallX = 1.f - wallX;
						}

						hitSide = RCRender::HideSide::DIG;
					}
					else {
						if (hitSide == RCRender::HideSide::NS) {
							wallX = _camera->Position.y + perpDistance * rayDirection.y;
						} else {
							wallX = _camera->Position.x + perpDistance * rayDirection.x;
						}
						wallX -= floor(wallX);
					}

					object.sideDistanceX  = sideDistanceX;
					object.sideDistanceY  = sideDistanceY;
					object.perpDistance   = perpDistance;
					object.deltaDistanceX = deltaDistanceX;
					object.deltaDistanceY = deltaDistanceY;
					object.unit           = mapUnit;
					object.hitSide        = hitSide;
					object.mapX           = mapX;
					object.mapY           = mapY;
					object.wallX          = wallX;
					objects[size] = object;
					++size;
					if (mapUnit.Type == RCMapUnitType::Door && mapUnit.Door->Max > mapUnit.Door->Offset) {
						continue;
					}
					if (mapUnit.Type == RCMapUnitType::Glass || mapUnit.Type == RCMapUnitType::Strip
					    || mapUnit.Type == RCMapUnitType::DiagWallLeftRight || mapUnit.Type == RCMapUnitType::DiagWallRightLeft) {
						continue;
					}
					else {
						furtherDistance = perpDistance;

						break;
					}
				}
			}
		}

		auto spriteList = _spriteMemoryPool.allocate(_scene->SpriteCount);
		int spriteCount = 0;
		for (int count = 0; count < _scene->SpriteCount; ++count) {
			RCRender::Sprite sprite{};
			auto spriteTarget = _scene->SpriteList[count];
			auto textureWidth   = spriteTarget->texture->_context->GetWidth();
			auto textureHeight  = spriteTarget->texture->_context->GetWidth();
			float spriteX = spriteTarget->x - _camera->Position.x;
			float spriteY = spriteTarget->y - _camera->Position.y;

			float invDet = 1.f / (_camera->Plane.x * _camera->Direction.y - _camera->Direction.x * _camera->Plane.y);

			float transformX = invDet * (_camera->Direction.y * spriteX - _camera->Direction.x * spriteY);
			sprite.transformY = invDet * (-_camera->Plane.y * spriteX + _camera->Plane.x * spriteY);

			if (sprite.transformY < 0) {
				continue;
			}

			int spriteScreenX = int(_renderTargetWidth / 2 * (1 + transformX / sprite.transformY));

			int vMoveScreen = int(spriteTarget->z / sprite.transformY);

			int spriteHeight = abs(int(_renderTargetHeight / sprite.transformY));
			sprite.drawStartY = -spriteHeight / 2 + _renderTargetHeight / 2 + vMoveScreen + Pitch + _camera->Z / sprite.transformY;
			sprite.drawEndY = spriteHeight / 2 + _renderTargetHeight / 2 + vMoveScreen + Pitch + _camera->Z / sprite.transformY;

			int spriteWidth = abs(int(_renderTargetHeight / sprite.transformY));
			sprite.drawStartX = -spriteWidth / 2 + spriteScreenX;
			sprite.drawEndX = spriteWidth / 2 + spriteScreenX;

			if (sprite.drawStartX >= _renderTargetWidth || sprite.drawEndX < 0) {
				continue;
			}

			// Precompute some variables for the vertical strips
			sprite.deltaY = sprite.drawEndY - sprite.drawStartY;
			sprite.countY = 0;
			sprite.textureY = 0;
			if (sprite.drawStartY < 0) {
				sprite.countY = -sprite.drawStartY * textureHeight;
				if (sprite.countY > sprite.deltaY) {
					div_t res = div(sprite.countY, sprite.deltaY);
					sprite.textureY += res.quot;
					sprite.countY = res.rem;
				}
				sprite.drawStartY = 0;
			}
			if (sprite.drawEndY >= _renderTargetHeight) {
				sprite.drawEndY = _renderTargetHeight - 1;
			}

			sprite.textureX = 0;
			sprite.deltaX = sprite.drawEndX - sprite.drawStartX;
			sprite.countX = 0;

			if (sprite.drawStartX < 0) {
				sprite.countX = -sprite.drawStartX * textureWidth;
				if (sprite.countX > sprite.deltaX)
				{
					div_t res = div(sprite.countX, sprite.deltaX);
					sprite.textureX += res.quot;
					sprite.countX = res.rem;
				}
				sprite.drawStartX = 0;
			}
			if (sprite.drawEndX > _renderTargetWidth) {
				sprite.drawEndX = _renderTargetWidth;
			}

			if (_scene->_enableFog) {
				sprite.fog = sprite.transformY / static_cast<float>(FogConstant) * _scene->_fogLevel;
			}

			sprite.texture = spriteTarget->texture;
			spriteList[count] = sprite;

			++spriteCount;
		}

		std::sort(spriteList, spriteList + _scene->SpriteCount, [](const RCRender::Sprite &Left, const RCRender::Sprite &Right) -> bool {
			return Left.transformY < Right.transformY;
		});

		int farSprite = spriteCount - 1;
		while (farSprite >= 0 && spriteList[farSprite].transformY > furtherDistance) {
			farSprite--;
		}
		for (int posCount = size - 1; posCount >= 0; --posCount) {
			auto mapUnit        = objects[posCount].unit;
			auto sideDistanceX  = objects[posCount].sideDistanceX;
			auto sideDistanceY  = objects[posCount].sideDistanceY;
			auto perpDistance   = objects[posCount].perpDistance;
			auto deltaDistanceX = objects[posCount].deltaDistanceX;
			auto deltaDistanceY = objects[posCount].deltaDistanceY;
			auto hitSide        = objects[posCount].hitSide;
			auto mapX           = objects[posCount].mapX;
			auto mapY           = objects[posCount].mapY;
			auto wallX          = objects[posCount].wallX;

			// 是否需要透明度混合
			bool transparentPass = mapUnit.Type == RCMapUnitType::Glass;

			int lineHeight = static_cast<int>(_renderTargetHeight / perpDistance);
			int drawStart  = -lineHeight / 2 + _renderTargetHeight / 2 + Pitch + _camera->Z / perpDistance;
			int drawEnd    = lineHeight / 2 + _renderTargetHeight / 2 + Pitch + _camera->Z / perpDistance;

			auto textureWidth  = mapUnit.Texture->_context->GetWidth();
			auto textureHeight = mapUnit.Texture->_context->GetHeight();
			int textureX       = static_cast<int>(wallX * double(textureWidth));
			// 如果是门，计算位移
			if (mapUnit.Type == RCMapUnitType::Door) {
				textureX -= textureWidth - mapUnit.Door->Offset;
				if (textureX < 0) {
					continue;
				}
			}
			int textureY = 0;
			int count    = 0;
			int deltaY   = drawEnd - drawStart;

			if ((hitSide == RCRender::HideSide::NS && rayDirection.x > 0) ||
			    (hitSide == RCRender::HideSide::EW && rayDirection.y < 0)) {
				textureX = textureWidth - textureX - 1;
			}

			float fog;
			if (_scene->_enableFog) {
				fog = perpDistance / FogConstant * _scene->_fogLevel;
			}
			if (drawStart < 0) {
				count = -drawStart * textureHeight;
				if (count > deltaY) {
					div_t divResult = div(count, deltaY);
					count           = divResult.rem;
					textureY += divResult.quot;
				}
				drawStart = 0;
			}
			if (drawEnd >= Height) {
				drawEnd = Height - 1;
			}

			while (farSprite >= 0 && spriteList[farSprite].transformY > perpDistance) {
				auto sprite = spriteList[farSprite];
				RenderSprite(sprite, x, sprite.fog);
				--farSprite;
			}
			for (int y = drawStart; y <= drawEnd; ++y) {
				COLORREF color = mapUnit.Texture->_buffer[textureY * textureWidth + textureX];
				// 如果 Alpha 通道不为零，则尝试绘制
				if (((color & 0xFF000000)) != 0) {
					// 明暗面处理
					if (hitSide == RCRender::HideSide::NS) {
						color = (color >> 1) & 8355711;
					} else if (hitSide == RCRender::HideSide::DIG) {
						color = (color >> 2) & 0x3F3F3F;
					}

					if (_scene->_enableFog) {
						auto &buffer = bufferPointer[y * _renderTargetWidth + x];
						if (fog >= 1) {
							color = _scene->_fogColor;
						} else if (fog > 0) {
							auto r = GetRValue(color) * (1 - fog) + _fogColorR * fog;
							auto g = GetGValue(color) * (1 - fog) + _fogColorG * fog;
							auto b = GetBValue(color) * (1 - fog) + _fogColorB * fog;

							color = RGB(r, g, b);
						}
					}

					if (transparentPass) {
						color = ((color & 0xFEFEFE) >> 1) + ((bufferPointer[y * _renderTargetWidth + x] & 0xFEFEFE) >> 1);
					}
					bufferPointer[y * _renderTargetWidth + x] = color;
				}

				count += textureHeight;
				while (count > deltaY) {
					++textureY;
					count -= deltaY;
				}
			}
		}
		while (farSprite >= 0) {
			RenderSprite(spriteList[farSprite], x, spriteList[farSprite].fog);
			--farSprite;
		}

		_mapObjectMemoryPool.deallocate(objects);
		_spriteMemoryPool.deallocate(spriteList);
	}
}
void RCRenderer::RenderSprite(RCRender::Sprite& sprite, const int &x, const float& fog) {
	auto bufferPointer = _enableResolution ? _resolutionRenderTarget->_backBuffer : _renderTarget->_backBuffer;
	auto spriteTextureHeight = sprite.texture->_context->GetHeight();
	auto spriteTextureWidth = sprite.texture->_context->GetWidth();
	if (x < sprite.drawStartX || x >= sprite.drawEndX) {
		return;
	}

	int delta = x - sprite.drawStartX;
	if (delta != 0) {
		sprite.drawStartX += delta;
		div_t res = div(sprite.countX + delta * spriteTextureHeight, sprite.deltaX);
		sprite.textureX += res.quot;
		sprite.countX = res.rem;
	}

	int spriteTextureY = sprite.textureY;
	int countY = sprite.countY;
	for (int y = sprite.drawStartY; y <= sprite.drawEndY; ++y)
	{
		COLORREF color = sprite.texture->_buffer[spriteTextureWidth * spriteTextureY + sprite.textureX];
		if (((color & 0xFF000000)) != 0)
		{
			if (_scene->_enableFog) {
				if (fog >= 1) {
					color = _scene->_fogColor;
				} else if (fog > 0) {
					auto r = GetRValue(color) * (1 - fog) + _fogColorR * fog;
					auto g = GetGValue(color) * (1 - fog) + _fogColorG * fog;
					auto b = GetBValue(color) * (1 - fog) + _fogColorB * fog;

					color = RGB(r, g, b);
				}
			}
			bufferPointer[y * _renderTargetWidth + x] = color;
		}

		countY += spriteTextureHeight;
		while (countY > sprite.deltaY)
		{
			++spriteTextureY;
			countY -= sprite.deltaY;
		}
	}

	++sprite.drawStartX;
	sprite.countX += spriteTextureWidth;
	while (sprite.countX > sprite.deltaX)
	{
		++sprite.textureX;
		sprite.countX -= sprite.deltaX;
	}
}
#ifdef _RC_RENDER_DEBUGER_
void RCRenderer::OutDebugText() {
	_renderTarget->Select();
	settextstyle(&_debuggerFont);
	settextcolor(BLACK);
	const std::basic_string<TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR>> stringList[] = {
	 _T("RCRenderer debugger information:"),
	 std::format(_T("  FPS : {}"), _fpsCount),
	 std::format(_T("  SkyBox : {}"), _scene->_enableSkybox ? _T("Enable") : _T("Disable")),
	 std::format(_T("  Fog : {}"), _scene->_enableFog ? _T("Enable") : _T("Disable")),
	 std::format(_T("  Fog Level : {}"), _scene->_fogLevel),
	 std::format(_T("  Enable X2 Super Resolution : {}"), _enableResolution ? _T("Enable") : _T("Disable")),
	 std::format(_T("RCEngine Camera information:")),
	 std::format(_T("  Pitch : {}"), _camera->_pitch),
	 std::format(_T("  Camera-Z : {}"), _camera->Z)
	};
	int heightCount = 10;
	settextcolor(BLACK);
	for (auto& text : stringList) {
		outtextxy(11, heightCount + 1, text.c_str());
		heightCount += 15;
	}
	heightCount = 10;
	settextcolor(WHITE);
	for (auto& text : stringList) {
		outtextxy(10, heightCount , text.c_str());
		heightCount += 15;
	}
}
#endif