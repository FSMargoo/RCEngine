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
 * \file RCTexture.h
 * \brief 对纹理的封装
 */

#pragma once

#include <include/RCContext.h>
#include <include/RCColor.h>

/**
 * 一个对纹理进行封装的类
 */
class RCTexture {
public:
	/**
	 * 由 RCContext 创建一个纹理对象
	 * @param Context 一个合法的纹理的 Context 指针
	 */
	explicit RCTexture(RCContext* Context);

public:
	/**
	 * 读取指定 buffer 下标的像素
	 * @param Position buffer 的下标
	 * @return COLORREF 格式的像素
	 */
#ifdef MSVC
	__forceinline
#else
	inline
#endif
	COLORREF ReadPixel(const int &Position) {
		return _buffer[Position];
	}

private:
	friend class RCRenderer;
	friend class RCMapDoor;

private:
	DWORD       *_buffer;
	RCContext   *_context;
};