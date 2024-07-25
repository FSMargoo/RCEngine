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
 * \file RCVideoWindow.h
 * \brief RC 引擎中对窗口以及消息的封装
 */

#pragma once

#include <graphics.h>
#include <include/RCRenderTarget.h>

/**
 * RC 引擎扩展的消息
 */
#define RCViewLookChange    4999
#define RCViewLookUp        5000
#define RCViewLookDown      5001
#define RCViewLookLeft      5002
#define RCViewLookRight     5003
#define RCViewReserve       5010
#define RCViewRecover       5011

/**
 * 对窗口以及消息的分装
 */
class RCVideoWindow {
public:
	/**
	 * 创建一个指定长、高和标题的窗口
	 * @param Width 窗口的长
	 * @param Height 窗口的高
	 * @param Title 窗口的标题
	 */
	RCVideoWindow(const int &Width, const int &Height, const TCHAR *Title);

public:
	/**
	 * 是否捕捉鼠标移动消息，并将鼠标移动消息改为视角移动消息。
	 * 注意，该设置为真后会隐藏鼠标并锁定鼠标至窗口中央
	 * @param Status 为真则会开始捕捉
	 */
	void SetCaptureCursorMove(bool Status);
	/**
	 * 尝试是否有消息
	 * @param message 指向目标消息结果对象的指针
	 * @return
	 */
	bool Message(ExMessage* Message);

public:
	/**
	 * 设置鼠标是否可见
	 * @param Status 如果是 true 则显示鼠标，如果为 false 则隐藏鼠标
	 */
	void SetCursorVisible(const bool &Status);
	/**
	 * 移动鼠标至窗口中央
	 */
	void MoveCursorToCenter();

public:
	/**
	 * 获得一个指向窗口的渲染目标、渲染内容的指针元组
	 * @return 指向窗口的渲染目标、渲染内容的指针元组，指针生命周期由用户自行管理
	 */
	std::tuple<RCRenderTarget*, RCContext*> GetRenderTuple();

private:
	friend class RCInteractor;

private:
	int     _width;
	int     _height;
	int     _halfWidth;
	int     _halfHeight;
	bool    _captureCursor;
	HWND    _handle;
};