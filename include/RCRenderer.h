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
 * \file RCRenderer.h
 * \brief RC 引擎中对场景的渲染封装
 */

#pragma once

#include <include/RCRenderTarget.h>
#include <include/RCCamera.h>
#include <include/RCScene.h>

#include <numbers>
#include <thirdparty/MemoryPool/C-11/MemoryPool.h>

namespace RCRender {
	/**
  	  * Render 内部使用的精灵对象
  	  */
	struct Sprite {
		RCTexture *texture;
		int drawStartX;
		int drawStartY;
		int drawEndX;
		int drawEndY;
		int deltaX;
		int deltaY;
		int countX;
		int countY;
		int textureX;
		int textureY;
		float transformY;
		float fog;
	};
	enum class HideSide {
		NS,
		EW,
		DIG
	};
	struct MapObject {
		float sideDistanceX;
		float sideDistanceY;
		float deltaDistanceX;
		float deltaDistanceY;
		float perpDistance;
		float wallX;
		int   mapX;
		int   mapY;
		RCMapUnit unit;
		HideSide hitSide;
	};
}

/**
 * RC 引擎中的场景渲染器
 */
class RCRenderer {
public:
	/**
	 * 构造一个渲染器，注意渲染器依赖
	 * @param RenderTarget 渲染器的渲染对象指针
	 * @param Camera 渲染器的相机指针
	 */
	RCRenderer(RCRenderTarget *RenderTarget, RCCamera *Camera, RCScene *Scene);

public:
	/**
	 * Pitch 的最大值，实际上 Pitch 的取值范围将为 [-PitchMax, PitchMax]
	 */
	float PitchMax;

public:
	/**
	 * 设置渲染器现在渲染的场景
	 * @param Scene 目标的渲染场景
	 */
	void SetScene(RCScene *Scene);

public:
	/**
	 * 启用超分渲染（插值超分），在高分辨率下建议启用超分渲染。
	 * 默认启用 2x 超分
	 * @param Status 当为 true 时，则启用超分渲染，否则禁用超分渲染
	 */
	void EnableSuperResolution(const bool &Status);

public:
	/**
	 * 在目标渲染器上渲染一帧，该函数会清空目标渲染器再进行渲染，
	 * 并在渲染结束后不会进行 flush
	 * @return 返回值为当前的理论帧率，用于交互器处理用户输入
	 */
	float Render();

private:
	/**
	 * 渲染地板
	 * @param Width 窗口宽度
	 * @param Height 窗口高度
	 * @param Pitch 计算后的 Pitch 常量
	 * @param FogConstant 烟雾的常量
	 * @param RayRightDirection 右平面向量
	 * @param RayLeftDirection 左平面向量
	 * @param CameraZ 相机的虚拟 Z 坐标
	 */
	void RenderFloor(const int &Width, const int &Height, const float &Pitch,
	                 const int& FogConstant, const vecmath::Vector<float>& RayRightDirection,
	                 const vecmath::Vector<float>& RayLeftDirection, const float& CameraZ,
	                 const int &Start);
	/**
	 * 渲染天花板
	 * @param Width 窗口宽度
	 * @param Height 窗口高度
	 * @param Pitch 计算后的 Pitch 常量
	 * @param FogConstant 烟雾的常量
	 * @param RayRightDirection 右平面向量
	 * @param RayLeftDirection 左平面向量
	 * @param CameraZ 相机的虚拟 Z 坐标
	 */
	void RenderCeiling(const int &Width, const int &Height, const float &Pitch,
	                   const int& FogConstant, const vecmath::Vector<float>& RayRightDirection,
	                   const vecmath::Vector<float>& RayLeftDirection, const float& CameraZ,
	                   const int &Start);
	/**
	 * 渲染天空盒
	 * @param Width 窗口宽度
	 * @param Height 窗口高度
	 * @param Pitch 计算后的 Pitch 常量
	 * @param FogConstant 烟雾的常量
	 * @param RayRightDirection 右平面向量
	 * @param RayLeftDirection 左平面向量
	 */
	void RenderSkyBox(const int &Width, const int &Height, const float &Pitch,
	                  const int& FogConstant, const vecmath::Vector<float>& RayRightDirection,
	                  const vecmath::Vector<float>& RayLeftDirection,
	                  const int &Start);
	/**
	 * 渲染墙体、玻璃、门、暗门等物体
	 * @param Width 窗口宽度
	 * @param Height 窗口高度
	 * @param Pitch 计算后的 Pitch 常量
	 * @param FogConstant 烟雾的常量
	 * @param RayRightDirection 右平面向量
	 * @param RayLeftDirection 左平面向量
	 */
	void RayCasting(const int &Width, const int &Height, const float &Pitch,
	                const int& FogConstant, const vecmath::Vector<float>& RayRightDirection,
	                const vecmath::Vector<float>& RayLeftDirection, const int &Start);
	/**
	 * 渲染精灵
	 */
	void RenderSprite(RCRender::Sprite& sprite, const int &x, const float& fog);


#ifdef _RC_RENDER_DEBUGER_
private:
	/**
	 * 输出调试信息
	 */
	 void OutDebugText();

private:
	 LOGFONT _debuggerFont;
	 time_t  _oldClock;
	 int     _fpsCount;
	 int     _fpsTemp;
#endif

private:
	static constexpr float pi = std::numbers::pi_v<float>;

private:
	/**
	 * 是否启用超分渲染，当启用超分渲染。渲染器会渲染一个比指定大小小 4 倍的图片并通过
	 * 直接放大来近似原结果
	 */
	bool             _enableResolution;
	RCContext       *_contextResolution;
	RCRenderTarget  *_resolutionRenderTarget;
	HDC              _resolutionDC;
	HDC              _renderTargetDC;
	int              _resolutionWidth;
	int              _resolutionHeight;
	float            _fogColorR;
	float            _fogColorG;
	float            _fogColorB;
	int              _renderTargetWidth;
	int              _renderTargetHeight;
	RCScene         *_scene;
	RCCamera        *_camera;
	RCRenderTarget  *_renderTarget;
	MemoryPool<RCRender::MapObject> _mapObjectMemoryPool;
	MemoryPool<RCRender::Sprite> _spriteMemoryPool;
};