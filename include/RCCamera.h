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
 * \file RCCamera.h
 * \brief 相机类
 */

#pragma once

#include <include/RCException.h>

#include <vecmath.hpp>

/**
 * 相机类，在 Ray Casting 中的相机
 */
class RCCamera {
public:
	/**
	 * 默认会创建一个位于 (0, 0)，场视角为 66° 的相机
	 */
	RCCamera();
	/**
	 * 依据指定信息创建一个相机
	 * @param IPosition 相机的位置
	 * @param IDirection 相机的方向
	 * @param Fov 相机的场视角，使用弧度制
	 */
	RCCamera(const vecmath::Vector<float> &IPosition, const vecmath::Vector<float> &IDirection,
	         const float &Fov);

public:
	/**
	 * 设置场视角
	 * @param Radian 弧度制场视角
	 */
	void SetFov(const float &Radian);
	/**
	 * 设置地平线距离
	 * @param Pitch 地平线位移百分比，取值范围为 [-1, 1]
	 */
	void SetPitch(const float &Pitch);

private:
	/**
	 * 初始化 Pitch 和 Z
	 */
	void InitCamera();

public:
	/**
	 * 相机位置
	 */
	vecmath::Vector<float> Position;
	/**
	 * 相机方向
	 */
	vecmath::Vector<float> Direction;
	/**
	 * 相机平面
	 */
	vecmath::Vector<float> Plane;
	/**
	 * 虚假的 Z 坐标，用于跳跃和下蹲
	 */
	float Z;


private:
	friend class RCRenderer;
	friend class RCInteractor;

private:
	/**
	 * 地平线位移（用于模拟上下视察），使用比例表示
	 * 取值范围为 [-1, 1]
	 */
	float _pitch;
};