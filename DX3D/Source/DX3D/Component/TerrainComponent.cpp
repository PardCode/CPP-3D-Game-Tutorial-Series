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

#include <DX3D/Component/TerrainComponent.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Math/MeshUtils.h>
#include <DX3D/Resource/TextureResource.h>
#include <DX3D/Graphics/Texture.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Component/TransformComponent.h>

#include <fstream>
#include <filesystem>
#include <ranges>


dx3d::TerrainComponent::TerrainComponent(const ComponentDesc& data) : Component(data)
{
	static auto res = generateTerrainMesh();
	m_terrainMesh = res;
}

void dx3d::TerrainComponent::setHeightMap(const RefPtr<TextureResource>& heightMap) noexcept
{
	m_heightMap = heightMap;
}

const dx3d::TextureResource* dx3d::TerrainComponent::getHeightMap() const noexcept
{
	return m_heightMap.get();
}

void dx3d::TerrainComponent::setFlatTexture(const RefPtr<TextureResource>& texture) noexcept
{
	m_flatTexture = texture;
}

const dx3d::TextureResource* dx3d::TerrainComponent::getFlatTexture() const noexcept
{
	return m_flatTexture.get();
}

void dx3d::TerrainComponent::setSlopeTexture(const RefPtr<TextureResource>& texture) noexcept
{
	m_slopeTexture = texture;
}

const dx3d::TextureResource* dx3d::TerrainComponent::getSlopeTexture() const noexcept
{
	return m_slopeTexture.get();
}

void dx3d::TerrainComponent::setSize(const Vec3& size) noexcept
{
	m_size = size;
}

dx3d::Vec3 dx3d::TerrainComponent::getSize() const noexcept
{
	return m_size;
}
const dx3d::VertexBuffer& dx3d::TerrainComponent::getVertexBuffer() const noexcept
{
	return *m_terrainMesh.vb;
}
const dx3d::IndexBuffer& dx3d::TerrainComponent::getIndexBuffer() const noexcept
{
	return *m_terrainMesh.ib;
}
const dx3d::GraphicsPipelineState& dx3d::TerrainComponent::getGraphicsPipelineState() const noexcept
{
	return *m_terrainMesh.pipelineState;
}


dx3d::f32 dx3d::TerrainComponent::getHeightFromWorldPos(const Vec3& worldPos) const noexcept
{
	const auto mapSize = m_heightMap->getSize();
	const auto scaleSize = m_size;

	const auto terrainWorldPos = getGameObject().getTransform().getPosition();
	const auto scale = Vec2(scaleSize.x / mapSize.width,  scaleSize.z / mapSize.height);
	const auto heightMapTexCoord = Vec2{
		(worldPos.x - terrainWorldPos.x) / scale.x,
		(worldPos.z - terrainWorldPos.z) / scale.y
	};

	if (heightMapTexCoord.x < 0 || heightMapTexCoord.y < 0)
		return 0;

	const auto x = static_cast<f32>(static_cast<ui32>(heightMapTexCoord.x)) ;
	const auto y = static_cast<f32>(static_cast<ui32>(heightMapTexCoord.y)) ;

	const auto deltaX = heightMapTexCoord.x - static_cast<ui32>(heightMapTexCoord.x);
	const auto deltaY = heightMapTexCoord.y - static_cast<ui32>(heightMapTexCoord.y);

	const auto height0 = m_heightMap->getPixelFromTexCoord({ x, y }).x;
	const auto height1 = m_heightMap->getPixelFromTexCoord({ x + 1, y }).x;

	const auto height2 = m_heightMap->getPixelFromTexCoord({ x, y + 1 }).x;
	const auto height3 = m_heightMap->getPixelFromTexCoord({ x + 1, y + 1 }).x;

	const auto heightX1 = MathUtils::lerp(height0, height1, deltaX);
	const auto heightX2 = MathUtils::lerp(height2, height3, deltaX);
	const auto height = MathUtils::lerp(heightX1, heightX2, deltaY) / 255.0f;

	return height * scaleSize.y;
}


bool dx3d::TerrainComponent::findTerrainHitPosition(const Vec3& pos, const Vec3& dir, f32 distance, f32 minHeight, Vec3& hitPosition) const noexcept
{
	if (distance <= 0.0f) return false;
	const auto step = distance / 10.0f;

	for (auto dist = 0.0f; dist <= distance; dist += step)
	{
		auto p = pos + dir * dist;

		const auto terrainHeight =
			getHeightFromWorldPos(p) + minHeight;

		if (p.y <= terrainHeight)
		{
			hitPosition = { p.x, terrainHeight, p.z };
			return true;
		}
	}

	return false;
}



dx3d::TerrainComponent::TerrainMesh dx3d::TerrainComponent::generateTerrainMesh()
{
	auto terrainMeshData = dx3d::MeshUtils::generateGridMesh(512, 512);
	auto vb = m_context.device.createVertexBuffer({ terrainMeshData.vertices.data(), static_cast<ui32>(terrainMeshData.vertices.size()), sizeof(MeshVertex)});
	auto ib = m_context.device.createIndexBuffer({ terrainMeshData.indices.data(),static_cast<ui32>(terrainMeshData.indices.size()) });

	auto shaderFileStr = "DX3D/Assets/Shaders/Terrain.hlsl";
	std::ifstream shaderStream(shaderFileStr);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file {}", shaderFileStr);
	const std::string shaderCode{
		std::istreambuf_iterator<char>(shaderStream),
		std::istreambuf_iterator<char>()
	};
	auto vsBinary = m_context.device.compileShader({ shaderFileStr, shaderCode.c_str(),
		shaderCode.size(), "_TerrainVSMain", ShaderType::VertexShader });
	auto psBinary = m_context.device.compileShader({ shaderFileStr, shaderCode.c_str(),
		shaderCode.size(), "_TerrainPSMain", ShaderType::PixelShader });
	auto layout = m_context.device.createGraphicsPipelineLayout({ vsBinary, psBinary });
	auto pipeline = m_context.device.createGraphicsPipelineState({ *layout });

	return { vb , ib, pipeline };
}