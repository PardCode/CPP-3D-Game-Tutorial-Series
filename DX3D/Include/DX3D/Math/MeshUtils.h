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
#include <DX3D/Core/Common.h>
#include <vector>
#include <ranges>
#include <cassert>

namespace dx3d
{
	namespace MeshUtils
	{
		struct GridMeshData
		{
			std::vector<MeshVertex> vertices{};
			std::vector<ui32> indices{};
		};

		inline auto generateGridMesh(ui32 width, ui32 height)
		{
			assert(width > 0 && "width cannot be 0");
			assert(height > 0 && "height cannot be 0");

			ui32 verticesCount = width * height;
			ui32 indicesCount = (width - 1) * (height - 1) * 6;

			std::vector<MeshVertex> vertices(verticesCount);
			std::vector<ui32> indices(indicesCount);

			// Generate vertices
			for (const ui32 y : std::views::iota(0u, height))
			{
				for (const ui32 x : std::views::iota(0u, width))
				{
					const ui32 vertexIndex = y * width + x;

					const f32 u = static_cast<f32>(x) / static_cast<f32>(width - 1);
					const f32 v = static_cast<f32>(y) / static_cast<f32>(height - 1);

					vertices[vertexIndex] = {
						Vec3(u, 0.0f, v),
						Vec2(u, v),
						Vec3()
					};
				}
			}

			// Generate indices
			ui32 index = 0;

			for (const ui32 y : std::views::iota(0u, height - 1))
			{
				for (const ui32 x : std::views::iota(0u, width - 1))
				{
					const ui32 topLeft = y * width + x;
					const ui32 topRight = topLeft + 1;
					const ui32 bottomLeft = (y + 1) * width + x;
					const ui32 bottomRight = bottomLeft + 1;

					indices[index++] = bottomLeft;
					indices[index++] = bottomRight;
					indices[index++] = topRight;

					indices[index++] = topRight;
					indices[index++] = topLeft;
					indices[index++] = bottomLeft;
				}
			}

			return GridMeshData{
				std::move(vertices),
				std::move(indices)
			};
		};
	}
}