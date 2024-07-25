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
 * \file RCMap.h
 * \brief RC 引擎的地图部分
 */

#pragma once

#include <include/RCTexture.h>

#include <vector>

/**
 * 地图中的门
 */
struct RCMapDoor {
public:
	RCMapDoor(RCTexture* Texture);

public:
	// 门的开关情况，即纹理的位移情况
	float   Offset;
	int     Max;
	int     Min;
	// 开门的速度
	int     Speed;

private:
	// 门的动画状态，若为 true 则为开启动画， false 则为关闭动画
	bool _animationStatus    = false;
	// 当前门是否处于动画状态
	bool _inAnimation        = false;

private:
	friend class RCInteractor;
};

/**
 * 地图单位的类型
 */
enum class RCMapUnitType {
	Air,  // 空气
	Wall, // 厚墙
	DiagWallLeftRight, // 45° 斜墙，从左上角到右下脚
	DiagWallRightLeft, // 45° 斜墙从右上角到左下角
	Door, // 门
	Strip,// 透明的门（条纹）
	Glass // 玻璃
};

/**
 * 地图的单位
 */
struct RCMapUnit {
	RCTexture       *Texture;
	RCMapUnitType    Type;
	RCMapDoor       *Door;
	bool             Passable = false;
};

/**
 * RC 引擎的地图，在 RC 引擎中，由于采用了 Ray Casting 的渲染方式，
 * 因此在 RC 引擎中地图其实是二维的，一个简单的 3x3 地图如下：
 * 			[ Wall, Wall, Wall ]
 * 			[ Wall, Air , Wall ]
 * 			[ Wall, Wall, Wall ]
 */
class RCMap {
public:
	/**
	 * 地图的构建函数
	 * @param Width 地图的长
	 * @param Height 地图的高
	 * @param mapPointer 一个指向地图数组的指针
	 */
	RCMap(const int &Width, const int &Height, RCMapUnit *MapPointer);

public:
	/**
	 * 获取地图指定下标的单位
	 * @param Position 指定的下标
	 * @return 一个指向地图单位的引用
	 */
	RCMapUnit& GetMapUnit(const int &Position);
	/**
	 * 获取地图的长
	 * @return 地图的长
	 */
	[[nodiscard]] int GetWidth() const;
	/**
	 * 获取地图的高
	 * @return 地图的高
	 */
	[[nodiscard]] int GetHeight() const;

private:
	friend class RCRenderer;
	friend class RCInteractor;

private:
	RCMapUnit  *_mapArray;
	int         _width;
	int         _height;
};