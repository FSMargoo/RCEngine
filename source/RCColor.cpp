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
 * \file RCColor.cpp
 * \brief RC 引擎中对颜色的封装
 */

#include <include/RCColor.h>

RCColor::RCColor(const Channel &Fill)
    : _r(Fill), _g(Fill), _b(Fill) {

}
RCColor::RCColor(const Channel &R, const Channel &G, const Channel &B)
	: _r(R), _g(G), _b(B) {

}
RCColor::RCColor() : _r(0), _g(0), _b(0) {
}
COLORREF RCColor::ToCOLORREF() const {
	return RGB(_r, _g, _b);
}
RCColor::Channel RCColor::GetR() const {
	return _r;
}
RCColor::Channel RCColor::GetG() const {
	return _g;
}
RCColor::Channel RCColor::GetB() const {
	return _b;
}
RCColor RCColor::MakeFromCOLORREF(const COLORREF &Color) {
	return RCColor{GetRValue(Color), GetGValue(Color), GetBValue(Color)};
}