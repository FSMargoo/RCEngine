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
 * \file RCInteractor.h
 * \brief RC 引擎的交互器，用于处理用户的鼠标、键盘输入
 */

#pragma once

#include <include/RCScene.h>
#include <include/RCCamera.h>
#include <include/RCVideoWindow.h>
#include <include/RCRenderer.h>

/**
 * 按键类型
 */
enum class RCInteractType {
	W, A, S, D, LookUp, LookDown, Sneak, Sprint, Jump, Interact, Quit
};

/**
 * RC 引擎用于处理用户鼠标、键盘输入的交互器
 */
class RCInteractor {
public:
	/**
	 * 创建交互器并绑定目标摄像机、窗口、地图、场景
	 * @param Camera 被交互器绑定的摄像机
	 * @param Window 被交互器绑定的窗口
	 * @param Map  被交互器绑定的地图
	 * @param Scene 被交互器绑定的场景
	 * @param XDelta 若处理鼠标移动，则为 X 轴的鼠标位移
	 * @param YDelta 若处理鼠标移动，则为 Y 轴的鼠标位移
	 */
	RCInteractor(RCCamera *Camera, RCVideoWindow *Window, RCMap *Map, RCScene *Scene);

public:
	/**
	 * 处理玩家与精灵的交互
	 */
	void SpriteInteractor();

private:
	/**
	 * 创建默认的键盘绑定
	 */
	void CreateDefaultKeyBind();

public:
	/**
	 * 玩家的移动速度
	 */
	float MoveSpeed;
	/**
	 * 玩家的上下视角移动速度（灵敏度）
	 */
	float PitchSpeed;
	/**
	 * 玩家的左右视角移动速度（灵敏度）
	 */
	float RotateSpeed;
	/**
	 * 玩家与交互物体最远的可交互距离
	 */
	float Reach;

public:
	/**
	 * 处理键鼠信息
	 */
	virtual void ProcessMessage(const ExMessage &Message, const float &FrameRate, const float &XDelta = 0,
	                            const float &YDelta = 0);
	/**
	 * 每一帧固定处理
	 */
	virtual void FrameProcess(const float &FrameRate);

public:
	void Interact(const float &frameRate);

public:
	/**
	 * 处理门的动画
	 */
	void ProcessDoorAnimation(const float &FrameRate);

private:
	/**
	 * 处理鼠标移动
	 */
	void CheckMouse(const int &halfWidth, const int &halfHeight, const float &frameRate);

private:
	std::vector<RCMapUnit*>                     _inAnimationDoor;
	RCMap*                                      _map;
	RCScene*                                    _scene;
	RCCamera*                                   _camera;
	int                                         _renderTargetWidth;
	float                                       _moveSpeedFactor;
	RCVideoWindow*                              _window;
	std::unordered_map<short, RCInteractType>   _keyBind;
	std::unordered_map<RCInteractType, bool>    _keyStatus;
};