/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

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
#include <DX3D/Math/DVec3.h>
#include <DX3D/Math/DVec2.h>

class DVertexMesh
{
public:
	DVertexMesh() :m_position(), m_texcoord(),m_normal()
	{
	}
	DVertexMesh(const DVec3& position, const DVec2& texcoord, const DVec3& normal, 
		const DVec3& tangent, const DVec3& binormal) :
		m_position(position), 
		m_texcoord(texcoord),
		m_normal(normal),
		m_tangent(tangent),
		m_binormal(binormal)
	{
	}
	DVertexMesh(const DVertexMesh& vertex) :
		m_position(vertex.m_position),
		m_texcoord(vertex.m_texcoord),
		m_normal(vertex.m_normal),
		m_tangent(vertex.m_tangent),
		m_binormal(vertex.m_binormal)
	{
	}

public:
	DVec3 m_position;
	DVec2 m_texcoord;
	DVec3 m_normal;
	DVec3 m_tangent;
	DVec3 m_binormal;
};