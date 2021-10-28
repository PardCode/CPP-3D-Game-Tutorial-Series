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
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector2D.h>

class VertexMesh
{
public:
	VertexMesh() :m_position(), m_texcoord(),m_normal()
	{
	}
	VertexMesh(const Vector3D& position, const Vector2D& texcoord, const Vector3D& normal, 
		const Vector3D& tangent, const Vector3D& binormal) :
		m_position(position), 
		m_texcoord(texcoord),
		m_normal(normal),
		m_tangent(tangent),
		m_binormal(binormal)
	{
	}
	VertexMesh(const VertexMesh& vertex) :
		m_position(vertex.m_position),
		m_texcoord(vertex.m_texcoord),
		m_normal(vertex.m_normal),
		m_tangent(vertex.m_tangent),
		m_binormal(vertex.m_binormal)
	{
	}

	~VertexMesh()
	{
	}

public:
	Vector3D m_position;
	Vector2D m_texcoord;
	Vector3D m_normal;
	Vector3D m_tangent;
	Vector3D m_binormal;
};