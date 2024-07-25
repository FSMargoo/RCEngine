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
 * \file RCContext.cpp
 * \brief 对 EasyX 的包装类，用于描述画布
 */

#include <include/RCContext.h>

RCContext::RCContext() {
	_context = nullptr;
	// 检查是否已经创建了窗口
	if (GetHWnd() == nullptr) {
		throw RCWindowHaveNotCreated();
	}
}
RCContext::RCContext(const int &Width, const int &Height) {
	_context = new IMAGE(Width, Height);
}
RCContext::RCContext(const TCHAR *ResourceType, const TCHAR *ResourceName) {
	loadimage(_context, ResourceType, ResourceName);
}
RCContext::RCContext(const TCHAR *FilePath) {
	_context = new IMAGE;
	loadimage(_context, FilePath);

	// 判断是否加载成功
	if (_context->getwidth() == 0 && _context->getheight() == 0) {
		throw RCCreationFailure("RCContext");
	}
}
int RCContext::GetWidth() const{
	if (_context != nullptr) {
		return _context->getwidth();
	}

	return getwidth();
}
int RCContext::GetHeight() const {
	if (_context != nullptr) {
		return _context->getheight();
	}

	return getheight();
}
void RCContext::Resize(const int &Width, const int &Height) {
	::Resize(_context, Width, Height);
}