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
 * \file RCScene.h
 * \brief 场景类包含一些有关场景的设置
 */

#pragma once

#include <include/RCSprite.h>
#include <include/RCMap.h>

/**
 * 场景类包含一些有关场景的设置，如天空盒，雾效果
 */
class RCScene {
public:
	/**
	 * 从地图构造场景，默认天空盒和烟雾效果将被禁用
	 * @param Map 一个指向合法的 RCMap 的指针
	 */
	explicit RCScene(RCMap *Map);

public:
	/**
	 * 设置天空盒贴图
	 * @param Texture 天空盒的贴图
	 */
	void SetSkyBoxTexture(RCTexture *Texture);
	/**
	 * 设置天花板贴图
	 * @param Texture 天花板的贴图
	 */
	void SetCeilingTexture(RCTexture *Texture);
	/**
	 * 设置地板贴图
	 * @param Texture 地板的贴图
	 */
	void SetFloorTexture(RCTexture *Texture);
	/**
	 * 设置烟雾效果的颜色
	 * @param Color COLORREF 格式烟雾的颜色
	 */
	void SetFogColor(const COLORREF &Color);
	/**
	 * 设置烟雾等级，等级越重，烟雾越浓
	 * @param Level 烟雾等级
	 */
	void SetFogLevel(const float &Level);
	/**
	 * 设置天空盒贴图的重复数
	 * @param Count 贴图重复数
	 */
	void SetSkyboxRepeat(const unsigned short &Count);

public:
	/**
	 * 启用天空盒，注意启用天空盒将会禁用天花板
	 * @param Status 若为 true 则启用天空盒，若为 false 则禁用天空盒
	 */
	void EnableSkyBox(const bool &Status);
	/**
	 * 启用烟雾效果
	 * @param Status 若为 true 则烟雾效果，若为 false 则禁用烟雾效果
	 */
	 void EnableFog(const bool &Status);

public:
	/**
	 * 检查场景是否合法，即检查纹理指针是否为空（若启用天空盒，则会检查是否设置天空盒纹理，否则
	 * 则检查是否设置了天花板纹理）
	 * @return 若场景合法，则返回 true，否则返回 false
	 */
	bool CheckValid();

public:
	/**
	 * 精灵列表
	 */
	RCSprite** SpriteList;
	int        SpriteCount;

private:
	friend class RCRenderer;
	friend class RCInteractor;

private:
	float        _fogLevel;
	int          _skyboxRepeats;
	bool         _enableSkybox;
	bool         _enableFog;
	COLORREF     _fogColor;
	RCTexture   *_skyBoxTexture;
	RCTexture   *_floorTexture;
	RCTexture   *_ceilingTexture;
	RCMap       *_map;
};