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
#include <DX3D/Component/TransformComponent.h>

#include <DX3D/Component/MeshComponent.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Resource/MaterialResource.h>
#include <DX3D/Resource/MeshResource.h>

#include <DX3D/Math/MeshUtils.h>

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