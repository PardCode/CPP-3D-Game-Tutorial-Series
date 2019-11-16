// Copyright (c) 2019  PardCode.
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.


#pragma once



class Point
{
public:
	Point() :m_x(0), m_y(0)
	{
	}
	Point(int x,int y):m_x(x),m_y(y)
	{
	}
	Point(const Point & point) :m_x(point.m_x), m_y(point.m_y)
	{
	}
	~Point()
	{
	}
public:
	int m_x = 0, m_y = 0;
};