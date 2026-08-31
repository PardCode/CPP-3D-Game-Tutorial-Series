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

#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Game/World.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec2.h>
#include <DX3D/Math/Mat4x4.h>
#include <DX3D/Graphics/GraphicsDevice.h>


dx3d::CubeComponent::CubeComponent(const ComponentDesc& data) : Component(data)
{
	static const MeshVertex vertexList[] =
	{
		// Front (+Z)
		{{-0.5f, -0.5f,  0.5f}, {0, 1},{0,0,1}},
		{{-0.5f,  0.5f,  0.5f}, {0, 0},{0,0,1}},
		{{ 0.5f,  0.5f,  0.5f}, {1, 0},{0,0,1}},
		{{ 0.5f, -0.5f,  0.5f}, {1, 1},{0,0,1}},

		// Back (-Z)
		{{ 0.5f, -0.5f, -0.5f}, {0, 1},{0,0,-1}},
		{{ 0.5f,  0.5f, -0.5f}, {0, 0},{0,0,-1}},
		{{-0.5f,  0.5f, -0.5f}, {1, 0},{0,0,-1}},
		{{-0.5f, -0.5f, -0.5f}, {1, 1},{0,0,-1}},

		// Left (-X)
		{{-0.5f, -0.5f, -0.5f}, {0, 1},{-1,0,0}},
		{{-0.5f,  0.5f, -0.5f}, {0, 0},{-1,0,0}},
		{{-0.5f,  0.5f,  0.5f}, {1, 0},{-1,0,0}},
		{{-0.5f, -0.5f,  0.5f}, {1, 1},{-1,0,0}},

		// Right (+X)
		{{ 0.5f, -0.5f,  0.5f}, {0, 1},{1,0,0}},
		{{ 0.5f,  0.5f,  0.5f}, {0, 0},{1,0,0}},
		{{ 0.5f,  0.5f, -0.5f}, {1, 0},{1,0,0}},
		{{ 0.5f, -0.5f, -0.5f}, {1, 1},{1,0,0}},

		// Top (+Y)
		{{-0.5f,  0.5f,  0.5f}, {0, 1},{0,1,0}},
		{{-0.5f,  0.5f, -0.5f}, {0, 0},{0,1,0}},
		{{ 0.5f,  0.5f, -0.5f}, {1, 0},{0,1,0}},
		{{ 0.5f,  0.5f,  0.5f}, {1, 1},{0,1,0}},

		// Bottom (-Y)
		{{-0.5f, -0.5f, -0.5f}, {0, 1},{0,-1,0}},
		{{-0.5f, -0.5f,  0.5f}, {0, 0},{0,-1,0}},
		{{ 0.5f, -0.5f,  0.5f}, {1, 0},{0,-1,0}},
		{{ 0.5f, -0.5f, -0.5f}, {1, 1},{0,-1,0}},
	};

	static const ui32 indexList[] =
	{
		 0,  2,  1,   0,  3,  2,   // Front
		 4,  6,  5,   4,  7,  6,   // Back
		 8, 10,  9,   8, 11, 10,   // Left
		12, 14, 13,  12, 15, 14,   // Right
		16, 18, 17,  16, 19, 18,   // Top
		20, 22, 21,  20, 23, 22    // Bottom
	};

	static const auto vb = m_context.device.createVertexBuffer({ vertexList, std::size(vertexList), sizeof(MeshVertex)});
	static const auto ib = m_context.device.createIndexBuffer({ indexList, std::size(indexList) });

	m_vb = vb;
	m_ib = ib;
}

void dx3d::CubeComponent::setMaterial(const RefPtr<MaterialResource>& material) noexcept
{
	m_material = material;
}

const dx3d::MaterialResource* dx3d::CubeComponent::getMaterial() const noexcept
{
	return m_material.get();
}

const dx3d::VertexBuffer& dx3d::CubeComponent::getVertexBuffer() const noexcept
{
	return *m_vb;
}

const dx3d::IndexBuffer& dx3d::CubeComponent::getIndexBuffer() const noexcept
{
	return *m_ib;
}
