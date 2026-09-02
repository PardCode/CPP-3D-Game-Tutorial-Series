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

#include <DX3D/Component/WaterComponent.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Resource/TextureResource.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Math/MeshUtils.h>
#include <fstream>
#include <filesystem>


dx3d::WaterComponent::WaterComponent(const ComponentDesc& data) : Component(data)
{
	static auto res = generateWaterMesh();
	m_waterMesh = res;
	m_wavesDisplacementTexture = m_context.resourceManager.createResourceFromFile<dx3d::TextureResource>(L"DX3D/Assets/Textures/waves_displacement.png");
}

const dx3d::TextureResource* dx3d::WaterComponent::getWavesDisplacementTexture() const noexcept
{
	return m_wavesDisplacementTexture.get();
}

void dx3d::WaterComponent::setWavesSpeed(f32 speed) noexcept
{
	m_wavesSpeed = speed;
}

dx3d::f32 dx3d::WaterComponent::getWavesSpeed() const noexcept
{
	return m_wavesSpeed;
}

void dx3d::WaterComponent::setSize(const Vec3& size) noexcept
{
	m_size = size;
}

dx3d::Vec3 dx3d::WaterComponent::getSize() const noexcept
{
	return m_size;
}
const dx3d::VertexBuffer& dx3d::WaterComponent::getVertexBuffer() const noexcept
{
	return *m_waterMesh.vb;
}
const dx3d::IndexBuffer& dx3d::WaterComponent::getIndexBuffer() const noexcept
{
	return *m_waterMesh.ib;
}
const dx3d::GraphicsPipelineState& dx3d::WaterComponent::getGraphicsPipelineState() const noexcept
{
	return *m_waterMesh.pipelineState;
}

dx3d::WaterComponent::WaterMesh dx3d::WaterComponent::generateWaterMesh()
{
	auto waterMeshData = dx3d::MeshUtils::generateGridMesh(1024, 1024);
	auto vb = m_context.device.createVertexBuffer({ waterMeshData.vertices.data(), static_cast<ui32>(waterMeshData.vertices.size()), sizeof(MeshVertex) });
	auto ib = m_context.device.createIndexBuffer({ waterMeshData.indices.data(),static_cast<ui32>(waterMeshData.indices.size()) });

	auto shaderFileStr = "DX3D/Assets/Shaders/Water.hlsl";
	std::ifstream shaderStream(shaderFileStr);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file {}", shaderFileStr);
	const std::string shaderCode{
		std::istreambuf_iterator<char>(shaderStream),
		std::istreambuf_iterator<char>()
	};
	auto vsBinary = m_context.device.compileShader({ shaderFileStr, shaderCode.c_str(),
		shaderCode.size(), "_WaterVSMain", ShaderType::VertexShader });
	auto psBinary = m_context.device.compileShader({ shaderFileStr, shaderCode.c_str(),
		shaderCode.size(), "_WaterPSMain", ShaderType::PixelShader });
	auto layout = m_context.device.createGraphicsPipelineLayout({ vsBinary, psBinary });
	auto pipeline = m_context.device.createGraphicsPipelineState({ *layout });

	return { vb , ib, pipeline };
}