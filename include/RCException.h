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
 * \file RCException.h
 * \brief RC 引擎中的异常类定义
 */

#pragma once

#include <format>
#include <iostream>
#include <stdexcept>

/**
 * RC 异常的基类
 */
class RCException : std::exception {
public:
	explicit RCException(const char *Message)
	    : std::exception(std::format("RCEngine exception : {}.", Message).c_str()) {
#ifdef _DEBUG
		std::cerr << std::format("RCEngine exception : {}.", Message);
#endif
	}
};
/**
 * 窗口未创建异常，用于窗口创建以前的非法操作
 */
class RCWindowHaveNotCreated : RCException {
public:
	RCWindowHaveNotCreated() : RCException("EasyX window haven't created") {
	}
};
/**
 * 创建对象失败异常
 */
class RCCreationFailure : RCException {
public:
	/**
	 * RCCreationFailure 构造函数
	 * @param Object 失败对象名称
	 */
	explicit RCCreationFailure(const char *Object)
	    : RCException(std::format("Failed to create {}", Object).c_str()) {
	}
};
/**
 * 非法参数异常
 */
class RCInvalidParameterException : RCException {
public:
	/**
	 * RCInvalidParameterException 构造函数
	 * @param InvalidString 异常补充
	 * @param Module 异常模块
	 */
	explicit RCInvalidParameterException(const char *InvalidString, const char *Module)
	    : RCException(std::format("Invalid parameter, {} is not accepted in {}",
	                              InvalidString, Module).c_str()) {
	}
};