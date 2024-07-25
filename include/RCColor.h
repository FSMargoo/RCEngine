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
 * \file RCColor.h
 * \brief RC 引擎中对颜色的封装
 */

#pragma once

#include <include/RCException.h>
#include <graphics.h>

/**
 * 对颜色的封装，该类会将 R、G、B 三通道分开保存
 */
class RCColor {
public:
	// 每个通道的取值范围为 [0, 255]，因此应使用 unsigned char 储存
	using Channel = unsigned char;

public:
	/**
	 * 将 R、G、B 三通道均赋为某值
	 * @param Fill 三通道的目标值
	 */
	explicit RCColor(const Channel &Fill);
	/**
	 * 通过三通道赋值以构建 RCColor 类
	 * @param R R 通道的颜色值
	 * @param G G 通道的颜色值
	 * @param B B 通道的颜色值
	 */
	RCColor(const Channel &R, const Channel &G, const Channel &B);
	/**
	 * 默认将会设置三通道为 0，即黑色
	 */
	RCColor();

public:
	/**
	 * 从 COLORREF 构造 RCColor 对象
	 * @param Color COLORREF 对象
	 * @return 一个从 COLORREF 构造而来的 RCColor 对象
	 */
	static RCColor MakeFromCOLORREF(const COLORREF &Color);

public:
	/**
	 * 获取 R 通道的数值
	 * @return R 通道的数值
	 */
	[[nodiscard]] Channel GetR() const;
	/**
	 * 获取 G 通道的数值
	 * @return G 通道的数值
	 */
	[[nodiscard]] Channel GetG() const;
	/**
	 * 获取 B 通道的数值
	 * @return B 通道的数值
	 */
	[[nodiscard]] Channel GetB() const;

public:
	/**
	 * 将 RCColor 转换为 COLORREF 形式
	 * @return 转换为 COLORREF 后的颜色
	 */
	[[nodiscard]] COLORREF ToCOLORREF() const;

public:
	friend RCColor operator+(const RCColor &Color, const RCColor &Value) {
		RCColor newColor(Color);

		newColor._r += Value._r;
		newColor._g += Value._g;
		newColor._b += Value._b;

		return newColor;
	}
	friend RCColor operator-(const RCColor &Color, const RCColor &Value) {
		RCColor newColor(Color);

		newColor._r -= Value._r;
		newColor._g -= Value._g;
		newColor._b -= Value._b;

		return newColor;
	}
	friend RCColor operator+(const RCColor &Color, const Channel &Value) {
		RCColor newColor(Color);

		newColor._r += Value;
		newColor._g += Value;
		newColor._b += Value;

		return newColor;
	}
	friend RCColor operator-(const RCColor &Color, const Channel &Value) {
		RCColor newColor(Color);

		newColor._r -= Value;
		newColor._g -= Value;
		newColor._b -= Value;

		return newColor;
	}
	friend RCColor operator*(const RCColor &Color, const Channel &Value) {
		RCColor newColor(Color);

		newColor._r *= Value;
		newColor._g *= Value;
		newColor._b *= Value;

		return newColor;
	}
	friend RCColor operator/(const RCColor &Color, const Channel &Value) {
		RCColor newColor(Color);

		newColor._r /= Value;
		newColor._g /= Value;
		newColor._b /= Value;

		return newColor;
	}
	friend RCColor operator*(const RCColor &Color, const float &Value) {
		RCColor newColor(Color);

		newColor._r = Value * newColor._r;
		newColor._g = Value * newColor._g;
		newColor._b = Value * newColor._b;

		return newColor;
	}
	friend RCColor operator/(const RCColor &Color, const float &Value) {
		RCColor newColor(Color);

		newColor._r = newColor._r / Value;
		newColor._g = newColor._g / Value;
		newColor._b = newColor._b / Value;

		return newColor;
	}
	friend RCColor operator+=(RCColor &Color, const Channel &Value) {
		Color._r += Value;
		Color._g += Value;
		Color._b += Value;

		return Color;
	}
	friend RCColor operator-=(RCColor &Color, const Channel &Value) {
		Color._r -= Value;
		Color._g -= Value;
		Color._b -= Value;

		return Color;
	}
	friend RCColor operator*=(RCColor &Color, const Channel &Value) {
		Color._r *= Value;
		Color._g *= Value;
		Color._b *= Value;

		return Color;
	}
	friend RCColor operator/=(RCColor &Color, const Channel &Value) {
		Color._r /= Value;
		Color._g /= Value;
		Color._b /= Value;

		return Color;
	}
	friend RCColor operator*=(RCColor &Color, const float &Value) {
		Color._r = Color._r * Value;
		Color._g = Color._g * Value;
		Color._b = Color._b * Value;

		return Color;
	}
	friend RCColor operator/=(RCColor &Color, const float &Value) {
		Color._r = Color._r / Value;
		Color._g = Color._g / Value;
		Color._b = Color._b / Value;

		return Color;
	}

private:
	Channel _r;
	Channel _g;
	Channel _b;
};