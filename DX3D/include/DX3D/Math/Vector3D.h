/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2021, PardCode

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

class Vector3D
{
public:
	Vector3D():x(0),y(0),z(0)
	{
	}
	Vector3D(float x,float y, float z) :x(x), y(y), z(z)
	{
	}
	Vector3D(const Vector3D& vector) :x(vector.x), y(vector.y), z(vector.z)
	{
	}

	static Vector3D lerp(const Vector3D& start, const Vector3D& end, float delta)
	{
		Vector3D v;
		v.x = start.x*(1.0f - delta) + end.x*(delta);
		v.y = start.y*(1.0f - delta) + end.y*(delta);
		v.z = start.z*(1.0f - delta) + end.z*(delta);
		return v;
	}

	Vector3D operator *(float num) const
	{
		return Vector3D(x*num, y*num, z*num);
	}

	Vector3D operator +(const Vector3D& vec) const
	{
		return Vector3D(x+vec.x, y+vec.y, z+vec.z);
	}

	Vector3D operator -(const Vector3D& vec) const
	{
		return Vector3D(x - vec.x, y - vec.y, z - vec.z);
	}

	float length() const
	{
		return (float)sqrt(x * x + y * y + z * z);
	}

	static Vector3D normalize(const Vector3D& vec)
	{
		Vector3D res;
		float len = (float)sqrt((float)(vec.x*vec.x) + (float)(vec.y*vec.y) + (float)(vec.z*vec.z));
		if (!len)
			return Vector3D();
		
		res.x = vec.x / len;
		res.y = vec.y / len;
		res.z = vec.z / len;

		return res;
	}

	static Vector3D cross(const Vector3D& v1, const Vector3D& v2)
	{
		Vector3D res;
		res.x = (v1.y * v2.z) - (v1.z * v2.y);
		res.y = (v1.z * v2.x) - (v1.x * v2.z);
		res.z = (v1.x * v2.y) - (v1.y * v2.x);
		return res;
	}

	~Vector3D()
	{
	}

public:
	float x, y, z;
};