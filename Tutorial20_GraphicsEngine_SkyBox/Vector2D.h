// Copyright (c) 2019 - 2020 PardCode
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#pragma once

class Vector2D
{
public:
	Vector2D() :m_x(0), m_y(0)
	{
	}
	Vector2D(float x, float y) :m_x(x), m_y(y)
	{
	}
	Vector2D(const Vector2D& vector) :m_x(vector.m_x), m_y(vector.m_y)
	{
	}

	Vector2D operator *(float num)
	{
		return Vector2D(m_x*num, m_y*num);
	}

	Vector2D operator +(Vector2D vec)
	{
		return Vector2D(m_x + vec.m_x, m_y + vec.m_y);
	}

	~Vector2D()
	{
	}

public:
	float m_x, m_y;
};