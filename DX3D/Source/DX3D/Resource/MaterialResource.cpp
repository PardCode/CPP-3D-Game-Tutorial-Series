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

#include <DX3D/Resource/MaterialResource.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Graphics/GraphicsPipelineLayout.h>

#include <fstream>
#include <filesystem>

dx3d::MaterialResource::MaterialResource(const MaterialResourceDesc& desc) : Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
	std::filesystem::path shaderFile = desc.base.path;

	auto shaderFileStr = shaderFile.string();
	std::ifstream shaderStream(shaderFile);
	if (!shaderStream) DX3DLogThrowError("Failed to open shader file {}", shaderFileStr.c_str());
	std::string shaderCode{
		std::istreambuf_iterator<char>(shaderStream),
		std::istreambuf_iterator<char>()
	};

	auto vsBinary = m_graphicsDevice.compileShader({ shaderFileStr.c_str(), shaderCode.c_str(),
		shaderCode.size(), "_VSMain", ShaderType::VertexShader });
	auto psBinary = m_graphicsDevice.compileShader({ shaderFileStr.c_str(), shaderCode.c_str(),
		shaderCode.size(), "_PSMain", ShaderType::PixelShader });

	m_layout = m_graphicsDevice.createGraphicsPipelineLayout({ vsBinary, psBinary });
	m_pipeline = m_graphicsDevice.createGraphicsPipelineState({ *m_layout });
	m_textures.resize(m_layout->getMaxTextureSlots());
}

dx3d::MaterialResource::MaterialResource(const MaterialResource& material, const MaterialResourceDesc& desc) : Resource(desc.base), m_graphicsDevice(desc.graphicsDevice)
{
	m_layout = material.m_layout;
	m_pipeline = material.m_pipeline;
	m_textures.resize(m_layout->getMaxTextureSlots());
}

const dx3d::GraphicsPipelineState& dx3d::MaterialResource::getGraphicsPipelineState() const noexcept
{
	return *m_pipeline;
}

void dx3d::MaterialResource::setData(const std::span<const std::byte>& data)
{
	if (!data.size())
	{
		DX3DLogError("No material data provided.")
		return;
	}
	if (data.size() > MaxDataSize)
	{
		DX3DLogWarning("Material data size ({} bytes) exceeds the maximum allowed size of {} bytes. Data will be truncated.", data.size(), MaxDataSize)
	}

	auto size = std::min(data.size(), MaxDataSize);
	memcpy(m_data, data.data(), size);
	m_dataSize = size;
}

const std::span<const std::byte> dx3d::MaterialResource::getData() const noexcept
{
	return m_data;
}

const dx3d::TextureResource* dx3d::MaterialResource::getTexture(size_t index) const
{
	if (index >= m_textures.size())
	{
		DX3DLogError("Index {} is out of bounds for list of size {}", index, m_textures.size());
		return {};
	}
	return m_textures[index].get();
}

size_t dx3d::MaterialResource::getNumTextures() const noexcept
{
	return m_textures.size();
}

void dx3d::MaterialResource::setTexture(size_t index, const dx3d::RefPtr<TextureResource>& texture)
{
	if (index >= m_textures.size())
	{
		DX3DLogError("Index {} is out of bounds for list of size {}", index, m_textures.size());
		return;
	}
	m_textures[index] = texture;
}
