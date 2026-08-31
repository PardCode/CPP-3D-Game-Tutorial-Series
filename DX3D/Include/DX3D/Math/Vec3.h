/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2026, PardCode

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
#include <DX3D/Core/Core.h>

namespace dx3d
{
	class Vec3
	{

	public:
		Vec3() = default;
		Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

		Vec3& operator+=(const Vec3& rhs) noexcept
		{
			x += rhs.x;
			y += rhs.y;
			z += rhs.z;
			return *this;
		}

		Vec3& operator*=(float scalar) noexcept
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		static Vec3 normalize(const Vec3& v) noexcept
		{
			float lenSq = v.x * v.x + v.y * v.y + v.z * v.z;

			if (lenSq == 0.0f) return Vec3{ 0.0f, 0.0f, 0.0f };

			float invLen = 1.0f / std::sqrt(lenSq);

			return Vec3{
				v.x * invLen,
				v.y * invLen,
				v.z * invLen
			};
		}
	public:
		f32 x{}, y{}, z{};
	};

	inline Vec3 operator+(Vec3 lhs, const Vec3& rhs) noexcept
	{
		lhs += rhs;
		return lhs;
	}

	inline Vec3 operator*(Vec3 v, float scalar) noexcept
	{
		v *= scalar;
		return v;
	}

	inline Vec3 operator*(float scalar, Vec3 v) noexcept
	{
		v *= scalar;
		return v;
	}
}

