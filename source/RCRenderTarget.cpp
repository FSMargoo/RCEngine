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
 * \file RCRenderTarget.cpp
 * \brief RC 引擎中的渲染对象封装
 */

#include <include/RCRenderTarget.h>

RCRenderTarget::RCRenderTarget(RCContext *Context) {
	if (Context == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCRenderTarget construction");
	}

	_context    = Context;
	_backBuffer = GetImageBuffer(_context->_context);
}
RCColor RCRenderTarget::ReadPixel(const int &X, const int &Y) {
	return RCColor::MakeFromCOLORREF(_backBuffer[_context->GetWidth() * Y + X]);
}
void RCRenderTarget::WritePixel(const int &X, const int &Y, const RCColor &Color) {
	_backBuffer[_context->GetWidth() * Y + X] = Color.ToCOLORREF();
}
RCColor RCRenderTarget::ReadPixel(const int &Position) {
	return RCColor::MakeFromCOLORREF(_backBuffer[Position]);
}
void RCRenderTarget::WritePixel(const int &Position, const RCColor &Color) {
	_backBuffer[Position] = Color.ToCOLORREF();
}
void RCRenderTarget::Select() {
	SetWorkingImage(_context->_context);
}
IMAGE* RCRenderTarget::GetImage() {
	return _context->_context;
}
RCContext* RCRenderTarget::GetContext() {
	return _context;
}
void RCRenderTarget::Flush() {
	if (_context->_context == nullptr) {
		FlushBatchDraw();
	}
}
void RCRenderTarget::Clear() {
	auto cache = GetWorkingImage();
	SetWorkingImage(_context->_context);
	cleardevice();
	SetWorkingImage(cache);
}