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
#include <DX3D/Game/Component.h>
#include <vector>

namespace dx3d
{
	class TerrainComponent final : public Component
	{
		dx3d_typeid(TerrainComponent)

		struct TerrainMesh
		{
			RefPtr<VertexBuffer> vb{};
			RefPtr<IndexBuffer> ib{};
			RefPtr<GraphicsPipelineState> pipelineState{};
		};

	public:
		explicit TerrainComponent(const ComponentDesc& data);

		void setHeightMap(const RefPtr<TextureResource>& map) noexcept;
		const TextureResource* getHeightMap() const noexcept;

		void setFlatTexture(const RefPtr<TextureResource>& texture) noexcept;
		const TextureResource* getFlatTexture() const noexcept;

		void setSlopeTexture(const RefPtr<TextureResource>& texture) noexcept;
		const TextureResource* getSlopeTexture() const noexcept;

		void setSize(const Vec3& size) noexcept;
		Vec3 getSize() const noexcept;


		const VertexBuffer& getVertexBuffer() const noexcept;
		const IndexBuffer& getIndexBuffer() const noexcept;
		const GraphicsPipelineState& getGraphicsPipelineState() const noexcept;

	private:
		TerrainMesh generateTerrainMesh();
	private:
		RefPtr<TextureResource> m_heightMap{};
		RefPtr<TextureResource> m_flatTexture{};
		RefPtr<TextureResource> m_slopeTexture{};

		Vec3 m_size{ 512, 100, 512 };

		TerrainMesh m_terrainMesh{};
	};
}

