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
 * \file RCVideoWindow.cpp
 * \brief RC 引擎中对窗口以及消息的封装
 */

#include <include/RCVideoWindow.h>

WNDPROC RCEasyXProc;
bool    RCShowCursor;

#define WM_FAKE_MESSAGE 0x9999

LRESULT CALLBACK RCWindowProc(HWND Handle, UINT Message, WPARAM wParam, LPARAM lParam) {
	switch (Message) {
		case WM_SETCURSOR:
			// 隐藏用户鼠标
			if (!RCShowCursor) {
				SetCursor(nullptr);

				// 欺骗 EasyX 不再处理鼠标信息
				Message = WM_FAKE_MESSAGE;
			}

			break;
	}
	return RCEasyXProc(Handle, Message, wParam, lParam);
}

RCVideoWindow::RCVideoWindow(const int &Width, const int &Height, const TCHAR *Title)
	: _captureCursor(false) {
	_handle = initgraph(Width, Height);

	_width      = Width;
	_height     = Height;
	_halfWidth  = _width / 2;
	_halfHeight = _height / 2;

	SetWindowText(_handle, Title);
	RCEasyXProc = (WNDPROC)GetWindowLongPtr(_handle, GWLP_WNDPROC);
	SetWindowLongPtr(_handle, GWLP_WNDPROC, (LONG_PTR)&RCWindowProc);
	BeginBatchDraw();

	RCShowCursor = true;
}
void RCVideoWindow::SetCursorVisible(const bool &Status) {
	RCShowCursor = Status;
}
void RCVideoWindow::MoveCursorToCenter() {
	POINT center(getwidth() / 2, getheight() / 2);
	ClientToScreen(_handle, &center);
	SetCursorPos(center.x, center.y);
}
void RCVideoWindow::SetCaptureCursorMove(bool Status) {
	_captureCursor = Status;
	SetCursorVisible(!Status);
}
std::tuple<RCRenderTarget*, RCContext*> RCVideoWindow::GetRenderTuple() {
	RCContext *windowContext = new RCContext();
	return { new RCRenderTarget(windowContext), windowContext };
}
bool RCVideoWindow::Message(ExMessage* Message) {
	auto result = peekmessage(Message);
	// 处理鼠标移动消息
	if (result && _captureCursor) {
		if (Message->message == WM_MBUTTONDOWN) {
			Message->message = RCViewReserve;
		}
		if (Message->message == WM_MBUTTONUP) {
			Message->message = RCViewRecover;
		}
	}

	return result;
}