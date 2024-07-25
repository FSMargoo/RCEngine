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
 * \file RCCamera.cpp
 * \brief 相机类
 */

#include <include/RCCamera.h>

RCCamera::RCCamera() {
	Position[0]  = 0;
	Position[1]  = 0;
	Direction[0] = -1;
	Direction[1] = 0;
	Plane[0]     = 0;
	Plane[1]     = 0.66;

	InitCamera();
}
RCCamera::RCCamera(const vecmath::Vector<float> &IPosition, const vecmath::Vector<float> &IDirection,
         const float &Fov) : Position(IPosition), Direction(IDirection) {
	SetFov(Fov);

	InitCamera();
}
void RCCamera::SetFov(const float &Radian) {
	float newLength = tan(Radian / 2);
	Plane           = newLength * Plane / Plane.length();
}
void RCCamera::SetPitch(const float &Pitch) {
	if (Pitch > 1.f || Pitch < -1.f) {
		throw RCInvalidParameterException("value out range of [-1, 1]", "RCCamera.SetPitch");
	}
	_pitch = Pitch;
}
void RCCamera::InitCamera() {
	_pitch    = 0;
	Z        = 0;
}