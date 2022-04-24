/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

Copyright (c) 2019-2022, PardCode

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

class  CXVec3
{
public:
 CXVec3(){}
 CXVec3(f32 x, f32 y, f32 z) :x(x), y(y), z(z){}
 CXVec3(const  CXVec3& vector) :x(vector.x), y(vector.y), z(vector.z){}

	static CXVec3 lerp(const  CXVec3& start, const  CXVec3& end, f32 delta)
	{
	 CXVec3 v;
		v.x = start.x*(1.0f - delta) + end.x*(delta);
		v.y = start.y*(1.0f - delta) + end.y*(delta);
		v.z = start.z*(1.0f - delta) + end.z*(delta);
		return v;
	}

 CXVec3 operator *(f32 num) const
	{
		return CXVec3(x*num, y*num, z*num);
	}

 CXVec3 operator +(const  CXVec3& vec) const
	{
		return CXVec3(x+vec.x, y+vec.y, z+vec.z);
	}

 CXVec3 operator -(const  CXVec3& vec) const
	{
		return CXVec3(x - vec.x, y - vec.y, z - vec.z);
	}

	f32 length() const
	{
		return (f32)sqrt(x * x + y * y + z * z);
	}

	static CXVec3 normalize(const  CXVec3& vec)
	{
	 CXVec3 res;
		f32 len = (f32)sqrt((f32)(vec.x*vec.x) + (f32)(vec.y*vec.y) + (f32)(vec.z*vec.z));
		if (!len)
			return CXVec3();
		
		res.x = vec.x / len;
		res.y = vec.y / len;
		res.z = vec.z / len;

		return res;
	}

	static CXVec3 cross(const  CXVec3& v1, const  CXVec3& v2)
	{
	 CXVec3 res;
		res.x = (v1.y * v2.z) - (v1.z * v2.y);
		res.y = (v1.z * v2.x) - (v1.x * v2.z);
		res.z = (v1.x * v2.y) - (v1.y * v2.x);
		return res;
	}

public:
	f32 x = 0.0f, y = 0.0f, z = 0.0f;
};