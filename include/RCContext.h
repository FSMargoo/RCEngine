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
 * \file RCContext.h
 * \brief 对 EasyX 的包装类，用于描述画布
 */

#pragma once

#include <include/RCException.h>

#include <graphics.h>

/**
 * 一个对 EasyX 的 IMAGE 包装类，用于描述画布。并添加了一些易于使用的 API
 */
class RCContext {
public:
	/**
	 * 默认将会创建一个代表当前窗口屏幕的 Context
	 */
	RCContext();
	/**
	 * 根据长、宽创建 Context
	 * @param Width Context 的长度
	 * @param Height Context 的宽度
	 */
	RCContext(const int &Width, const int &Height);
	/**
	 * 从资源文件创建 Context
	 * @param ResourceType 资源类型
	 * @param ResourceName 资源名称
	 */
	RCContext(const TCHAR *ResourceType, const TCHAR *ResourceName);
	/**
	 * 从图片文件创建 Context，注意，由于该构造函数依靠图片长宽是否
	 * 等于零以判断是否成功加载文件，所以请不要传入长宽都为零的图片
	 * @param FilePath 指向受支持的图片文件路径
	 */
	explicit RCContext(const TCHAR *FilePath);

public:
	/**
	 * 获取 Context 的宽
	 * @return 该 Context 目前的宽
	 */
	[[nodiscard]] int GetWidth() const;
	/**
	 * 获取 Context 的高
	 * @return 该 Context 目前的高
	 */
	[[nodiscard]] int GetHeight() const;

public:
	/**
	 * 修改当前 context 的大小
	 * @param Width 新的宽
	 * @param Height 新的高
	 */
	void Resize(const int &Width, const int &Height);

private:
	friend class RCRenderTarget;
	friend class RCTexture;
	friend class RCRenderer;

private:
	IMAGE *_context;
};