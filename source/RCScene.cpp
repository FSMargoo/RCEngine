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
 * \file RCScene.cpp
 * \brief 场景类包含一些有关场景的设置
 */

#include <include/RCScene.h>

RCScene::RCScene(RCMap *Map)
    : _map(Map), _skyBoxTexture(nullptr), _floorTexture(nullptr), _ceilingTexture(nullptr),
	 _fogColor(0xA09EE7), _enableSkybox(false), _enableFog(false), _skyboxRepeats(1) {
	if (Map == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCScene construction");
	}
}
void RCScene::SetSkyBoxTexture(RCTexture *Texture) {
	if (Texture == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCScene.SetSkyBox");
	}\
	_skyBoxTexture = Texture;
}
void RCScene::SetCeilingTexture(RCTexture *Texture) {
	if (Texture == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCScene.SetCeilingTexture");
	}
	_ceilingTexture = Texture;
}
void RCScene::SetFloorTexture(RCTexture *Texture) {
	if (Texture == nullptr) {
		throw RCInvalidParameterException("nullptr", "RCScene.SetFloorTexture");
	}
	_floorTexture = Texture;
}
void RCScene::SetFogColor(const COLORREF &Color) {
	_fogColor = BGR(Color);
}
void RCScene::SetFogLevel(const float &Level) {
	_fogLevel = Level;
}
void RCScene::SetSkyboxRepeat(const unsigned short &Count) {
	_skyboxRepeats = Count;
}
void RCScene::EnableSkyBox(const bool &Status) {
	_enableSkybox = Status;
}
void RCScene::EnableFog(const bool &Status) {
	_enableFog = Status;
}
bool RCScene::CheckValid() {
	bool flag = ((_enableSkybox) ? _skyBoxTexture != nullptr : _ceilingTexture != nullptr) &&
	        _floorTexture != nullptr && _map != nullptr;
	return flag;
}