/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

Copyright (c) 2019-2024, PardCode

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#pragma once
#include <CX3D/CXPrerequisites.h>


class  CXRect
{
public:
	CXRect() {}
	CXRect(i32 width, i32 height) : width(width), height(height) {}
	CXRect(i32 left, i32 top, i32 width, i32 height) :left(left), top(top), width(width), height(height) {}
	CXRect(const  CXRect& rect) :left(rect.left), top(rect.top), width(rect.width), height(rect.height) {}

public:
	i32 width = 0, height = 0, left = 0, top = 0;
};