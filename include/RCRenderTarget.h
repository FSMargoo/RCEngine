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
 * \file RCRenderTarget.h
 * \brief RC 引擎中的渲染对象封装
 */

#pragma once

#include <include/RCContext.h>
#include <include/RCColor.h>

/**
 * 对渲染对象进行封装，提供像素遍历接口
 */
class RCRenderTarget {
public:
	/**
	 * RCRenderTarget 需要指向一个合法的 RCContext 对象
	 * @param Context 目标 RCContext 对象的指针
	 */
	explicit RCRenderTarget(RCContext *Context);
	~RCRenderTarget() {

	}

public:
	/**
	 * 读取指定地点的像素信息，注意，出于性能考虑，本函数不会进行越界检查
	 * @param X 目标像素的 X 坐标
	 * @param Y 目标像素的 Y 坐标
	 * @return 目标的颜色，以 RCColor 返回
	 */
	RCColor ReadPixel(const int &X, const int &Y);
	/**
	 * 写入指定地点的像素信息，注意，出于性能考虑，本函数不会进行越界检查
	 * @param X 目标像素的 X 坐标
	 * @param Y 目标像素的 Y 坐标
	 * @param Color 写入的目标颜色
	 */
	void WritePixel(const int &X, const int &Y, const RCColor &Color);
	/**
	 * 读取指定地点的像素信息，注意，出于性能考虑，本函数不会进行越界检查
	 * @param Position 目标像素的 buffer 下标
	 * @return 目标的颜色，以 RCColor 返回
	 */
	RCColor ReadPixel(const int &Position);
	/**
	 * 写入指定地点的像素信息，注意，出于性能考虑，本函数不会进行越界检查
	 * @param Position 目标像素的 buffer 下标
	 * @param Color 写入的目标颜色
	 */
	void WritePixel(const int &Position, const RCColor &Color);
	/**
	 * 写入指定地点的像素信息，注意，出于性能考虑，本函数不会进行越界检查
	 * @param Position 目标像素的 buffer 下标
	 * @param Color 写入的目标颜色
	 */
	inline void WritePixel(const int &Position, const COLORREF &Color) {
		_backBuffer[Position] = Color;
	}
	/**
	 * 加载当前 RenderTarget 至 EasyX 的工作区中
	 */
	void Select();
	/**
	 * 获取当前 RenderTarget 的 EasyX 对象
	 * @return RenderTarget 的 EasyX 对象
	 */
	IMAGE* GetImage();
	/**
	 * 获取当前 RenderTarget 的 RCContext 对象
	 * @return RenderTarget 的 RCContext 对象
	 */
	RCContext* GetContext();

public:
	/**
	 * 刷新渲染对象，注意，当渲染对象为窗口时将调用批量绘图接口，
	 * 而当渲染对象为图像时，该函数将无任何行为
	 */
	void Flush();
	/**
	 * 清空渲染对象
	 */
	void Clear();

private:
	friend class RCRenderer;

private:
	DWORD       *_backBuffer;
	RCContext   *_context;
};