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
 * \file RCInteractor.cpp
 * \brief RC 引擎的交互器，用于处理用户的鼠标、键盘输入
 */

#include <include/RCInteractor.h>

RCInteractor::RCInteractor(RCCamera *Camera, RCVideoWindow *Window, RCMap *Map, RCScene *Scene)
    : _camera(Camera), _window(Window), _map(Map), MoveSpeed(4.5f), PitchSpeed(1.8f), RotateSpeed(3.1415926 / 2),
		_scene(Scene), Reach(2.2f), _moveSpeedFactor(1.f) {
	CreateDefaultKeyBind();

	_renderTargetWidth = Window->_width;
}
void RCInteractor::SpriteInteractor() {
	for (int count = 0; count < _scene->SpriteCount; ++count) {
		auto sprite = _scene->SpriteList[count];
		auto distance = sqrt(pow(sprite->x - _camera->Position.x, 2) + pow(sprite->y - _camera->Position.y, 2));
		if (distance < sprite->triggerRange && sprite->interactable) {
			sprite->OnTrigger();
		}
	}
}
void RCInteractor::CreateDefaultKeyBind() {
	_keyBind.insert({ 'W', RCInteractType::W });
	_keyBind.insert({ 'A', RCInteractType::A });
	_keyBind.insert({ 'S', RCInteractType::S });
	_keyBind.insert({ 'D', RCInteractType::D });
	_keyBind.insert({ 17, RCInteractType::Sneak });
	_keyBind.insert({ 16, RCInteractType::Sprint });
	_keyBind.insert({ 'F', RCInteractType::Interact });
}
void RCInteractor::ProcessMessage(const ExMessage &Message, const float &FrameRate, const float &XDelta,
                                  const float &YDelta) {
	auto xDelta = abs(XDelta);
	auto yDelta = abs(YDelta);
	switch (Message.message) {
		case WM_KEYDOWN: {
			auto keyBind = _keyBind.find(Message.vkcode);
			if (keyBind != _keyBind.end()) {
				switch (keyBind->second) {
					case RCInteractType::Interact: {
						for (int x = 0; x < _renderTargetWidth; ++x) {
							float cameraX                       = 2.f * static_cast<float>(x) / static_cast<float>(_renderTargetWidth) - 1;
							vecmath::Vector<float> rayDirection = _camera->Direction + _camera->Plane * cameraX;
							int mapX                            = static_cast<int>(_camera->Position.x);
							int mapY                            = static_cast<int>(_camera->Position.y);

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

							// 检查玩家指向的位置是否有可交互的物体
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
								if (mapUnit.Type != RCMapUnitType::Air && mapUnit.Type != RCMapUnitType::Door) {
									break;
								}
								else if (mapUnit.Type != RCMapUnitType::Air) {
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
									if (perpDistance > Reach) {
										break;
									}
									else {
										if (!mapUnit.Door->_inAnimation) {
											mapUnit.Door->_animationStatus = !mapUnit.Door->_animationStatus;
											mapUnit.Door->_inAnimation     = true;

											_inAnimationDoor.push_back(&_scene->_map->_mapArray[mapX + mapY * _scene->_map->_width]);
										}
										break;
									}
								}
							}
						}

						break;
					}
					case RCInteractType::W: {
						_keyStatus[RCInteractType::W] = true;

						break;
					}
					case RCInteractType::S: {
						_keyStatus[RCInteractType::S] = true;

						break;
					}
					case RCInteractType::D: {
						_keyStatus[RCInteractType::D] = true;


						break;
					}
					case RCInteractType::A: {
						_keyStatus[RCInteractType::A] = true;

						break;
					}
					case RCInteractType::Sneak: {
						_camera->Z = -100.f;
						_moveSpeedFactor = 0.3f;

						break;
					}
					case RCInteractType::Sprint: {
						_moveSpeedFactor = 2.f;
					}
				}
			}

			break;
		}
		case WM_KEYUP: {
			auto keyBind = _keyBind.find(Message.vkcode);
			if (keyBind != _keyBind.end()) {
				switch (keyBind->second) {
					case RCInteractType::Sprint: {
						_moveSpeedFactor = 1.f;
					}
					case RCInteractType::Sneak: {
						_camera->Z = 0;
						_moveSpeedFactor = 1.f;

						break;
					}
					case RCInteractType::W: {
						_keyStatus[RCInteractType::W] = false;

						break;
					}
					case RCInteractType::S: {
						_keyStatus[RCInteractType::S] = false;

						break;
					}
					case RCInteractType::D: {
						_keyStatus[RCInteractType::D] = false;


						break;
					}
					case RCInteractType::A: {
						_keyStatus[RCInteractType::A] = false;

						break;
					}
				}
			}

			break;
		}
		case RCViewLookChange: {
			auto actualSpeed = RotateSpeed * FrameRate;
			// 如果窗口允许捕捉鼠标信息
			switch (Message.x) {
				case RCViewLookLeft: {
					vecmath::Matrix<float> rotationMatrix(
					        vecmath::Vector<float>(cos(actualSpeed * xDelta), -sin(actualSpeed * xDelta), 0),
					        vecmath::Vector<float>(sin(actualSpeed * xDelta), cos(actualSpeed * xDelta), 0),
					        vecmath::Vector<float>(0, 0, 0)
					);
					_camera->Direction  = rotationMatrix.transform(_camera->Direction);
					_camera->Plane      = rotationMatrix.transform(_camera->Plane);

					break;
				}
				case RCViewLookRight: {
					vecmath::Matrix<float> rotationMatrix(
					        vecmath::Vector<float>(cos(-actualSpeed * xDelta), -sin(-actualSpeed * xDelta), 0),
					        vecmath::Vector<float>(sin(-actualSpeed * xDelta), cos(-actualSpeed * xDelta), 0),
					        vecmath::Vector<float>(0, 0, 0)
					);
					_camera->Direction  = rotationMatrix.transform(_camera->Direction);
					_camera->Plane      = rotationMatrix.transform(_camera->Plane);

					break;
				}
			}
			switch (Message.y) {
				case RCViewLookUp: {
					_camera->_pitch += PitchSpeed * actualSpeed * yDelta;
					if (_camera->_pitch > 1.f) {
						_camera->_pitch = 1.f;
					}

					break;
				}
				case RCViewLookDown: {
					_camera->_pitch -= PitchSpeed * actualSpeed * yDelta;
					if (_camera->_pitch < -1.f) {
						_camera->_pitch = -1.f;
					}

					break;
				}
			}

			break;
		}
	}
}
void RCInteractor::FrameProcess(const float &FrameRate) {
	auto actualSpeed = MoveSpeed * FrameRate * _moveSpeedFactor;
	if (_keyStatus[RCInteractType::W]) {
		auto xDelta = _camera->Direction.x * actualSpeed;
		auto yDelta = _camera->Direction.y * actualSpeed;
		auto mapUnitX = _map->_mapArray[_map->_width * int(_camera->Position.y) + int(_camera->Position.x + xDelta)];
		auto mapUnitY = _map->_mapArray[_map->_width * int(_camera->Position.y + yDelta) + int(_camera->Position.x)];
		bool doorPassableX = mapUnitX.Type == RCMapUnitType::Door && mapUnitX.Door->Offset == mapUnitX.Door->Min && !mapUnitX.Door->_inAnimation;
		bool doorPassableY = mapUnitY.Type == RCMapUnitType::Door && mapUnitY.Door->Offset == mapUnitY.Door->Min && !mapUnitY.Door->_inAnimation;
		if (mapUnitX.Passable || mapUnitX.Type == RCMapUnitType::Air || doorPassableX) {
			_camera->Position.x += xDelta;
		}
		if (mapUnitY.Passable || mapUnitY.Type == RCMapUnitType::Air || doorPassableY) {
			_camera->Position.y += yDelta;
		}
	}
	if (_keyStatus[RCInteractType::S]) {
		auto xDelta = _camera->Direction.x * -actualSpeed;
		auto yDelta = _camera->Direction.y * -actualSpeed;
		auto mapUnitX = _map->_mapArray[_map->_width * int(_camera->Position.y) + int(_camera->Position.x + xDelta)];
		auto mapUnitY = _map->_mapArray[_map->_width * int(_camera->Position.y + yDelta) + int(_camera->Position.x)];
		bool doorPassableX = mapUnitX.Type == RCMapUnitType::Door && mapUnitX.Door->Offset == mapUnitX.Door->Min && !mapUnitX.Door->_inAnimation;
		bool doorPassableY = mapUnitY.Type == RCMapUnitType::Door && mapUnitY.Door->Offset == mapUnitY.Door->Min && !mapUnitY.Door->_inAnimation;
		if (mapUnitX.Passable || mapUnitX.Type == RCMapUnitType::Air || doorPassableX) {
			_camera->Position.x += xDelta;
		}
		if (mapUnitY.Passable || mapUnitY.Type == RCMapUnitType::Air || doorPassableY) {
			_camera->Position.y += yDelta;
		}
	}
	if (_keyStatus[RCInteractType::D]) {
		// 得到垂直的相机 direction
		vecmath::Matrix<float> rotationMatrix(
		        vecmath::Vector<float>(cos(3.1415926f / 2.f), -sin(3.1415926f / 2.f), 0),
		        vecmath::Vector<float>(sin(3.1415926f / 2.f), cos(3.1415926f / 2.f), 0),
		        vecmath::Vector<float>(0, 0, 0)
		);
		auto perpDirection = rotationMatrix.transform(_camera->Direction);
		auto xDelta = perpDirection.x * -actualSpeed;
		auto yDelta = perpDirection.y * -actualSpeed;
		auto mapUnitX = _map->_mapArray[_map->_width * int(_camera->Position.y) + int(_camera->Position.x + xDelta)];
		auto mapUnitY = _map->_mapArray[_map->_width * int(_camera->Position.y + yDelta) + int(_camera->Position.x)];
		bool doorPassableX = mapUnitX.Type == RCMapUnitType::Door && mapUnitX.Door->Offset == mapUnitX.Door->Min && !mapUnitX.Door->_inAnimation;
		bool doorPassableY = mapUnitY.Type == RCMapUnitType::Door && mapUnitY.Door->Offset == mapUnitY.Door->Min && !mapUnitY.Door->_inAnimation;
		if (mapUnitX.Passable || mapUnitX.Type == RCMapUnitType::Air || doorPassableX) {
			_camera->Position.x += xDelta;
		}
		if (mapUnitY.Passable || mapUnitY.Type == RCMapUnitType::Air || doorPassableY) {
			_camera->Position.y += yDelta;
		}
	}
	if (_keyStatus[RCInteractType::A]) {
		// 得到垂直的相机 direction
		vecmath::Matrix<float> rotationMatrix(
		        vecmath::Vector<float>(cos(3.1415926f / 2.f), -sin(3.1415926f / 2.f), 0),
		        vecmath::Vector<float>(sin(3.1415926f / 2.f), cos(3.1415926f / 2.f), 0),
		        vecmath::Vector<float>(0, 0, 0)
		);
		auto perpDirection = rotationMatrix.transform(_camera->Direction);
		auto xDelta = perpDirection.x * actualSpeed;
		auto yDelta = perpDirection.y * actualSpeed;
		auto mapUnitX = _map->_mapArray[_map->_width * int(_camera->Position.y) + int(_camera->Position.x + xDelta)];
		auto mapUnitY = _map->_mapArray[_map->_width * int(_camera->Position.y + yDelta) + int(_camera->Position.x)];
		bool doorPassableX = mapUnitX.Type == RCMapUnitType::Door && mapUnitX.Door->Offset == mapUnitX.Door->Min && !mapUnitX.Door->_inAnimation;
		bool doorPassableY = mapUnitY.Type == RCMapUnitType::Door && mapUnitY.Door->Offset == mapUnitY.Door->Min && !mapUnitY.Door->_inAnimation;
		if (mapUnitX.Passable || mapUnitX.Type == RCMapUnitType::Air || doorPassableX) {
			_camera->Position.x += xDelta;
		}
		if (mapUnitY.Passable || mapUnitY.Type == RCMapUnitType::Air || doorPassableY) {
			_camera->Position.y += yDelta;
		}
	}
}
void RCInteractor::ProcessDoorAnimation(const float &FrameRate) {
	for (int count = 0; count < _inAnimationDoor.size(); ++count) {
		float offsetSymbol = _inAnimationDoor[count]->Door->_animationStatus ? -1.f : 1.f;
		_inAnimationDoor[count]->Door->Offset += offsetSymbol * _inAnimationDoor[count]->Door->Speed * FrameRate;
		_inAnimationDoor[count]->Door->Offset = _inAnimationDoor[count]->Door->Offset < _inAnimationDoor[count]->Door->Min ? _inAnimationDoor[count]->Door->Min : _inAnimationDoor[count]->Door->Offset;
		_inAnimationDoor[count]->Door->Offset = _inAnimationDoor[count]->Door->Offset < static_cast<float>(_inAnimationDoor[count]->Door->Max) ?
		                                                                                                                   _inAnimationDoor[count]->Door->Offset : _inAnimationDoor[count]->Door->Max;
		if (!_inAnimationDoor[count]->Door->_animationStatus &&
		    _inAnimationDoor[count]->Door->Offset == _inAnimationDoor[count]->Door->Max) {
			_inAnimationDoor[count]->Door->_inAnimation = false;
			_inAnimationDoor.erase(_inAnimationDoor.begin() + count);
			--count;
			continue;
		}
		if (_inAnimationDoor[count]->Door->_animationStatus &&
		    _inAnimationDoor[count]->Door->Offset == _inAnimationDoor[count]->Door->Min) {
			_inAnimationDoor[count]->Door->_inAnimation = false;
			_inAnimationDoor.erase(_inAnimationDoor.begin() + count);
			--count;
			continue;
		}
	}
}