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
 * \file RCMap.cpp
 * \brief RC 引擎的地图部分
 */

#include <include/RCMap.h>

RCMapDoor::RCMapDoor(RCTexture* Texture) : Offset(Texture->_context->GetWidth()), Max(Texture->_context->GetWidth()), Speed(40), Min(Texture->_context->GetWidth() / 6) {

}
RCMap::RCMap(const int &Width, const int &Height, RCMapUnit *MapPointer)
	: _width(Width), _height(Height), _mapArray(MapPointer) {
	if (_mapArray == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCMap construction");
	}
}
RCMapUnit& RCMap::GetMapUnit(const int &Position) {
	return _mapArray[Position];
}
int RCMap::GetWidth() const {
	return _width;
}
int RCMap::GetHeight() const {
	return _height;
}