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

#include <DX3D/Resource/TextureResource.h>
#include <DX3D/Resource/ResourceManager.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <fstream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include <stb-image/stb_image.h>


dx3d::TextureResource::TextureResource(const TextureResourceDesc& desc) : Resource(desc.base)
{
	std::filesystem::path textureFile = desc.base.path;
	auto textureFileStr = textureFile.string();

	auto width{0}, height{0}, channels{0};
	auto pixels = stbi_load(
		textureFileStr.c_str(),
		&width,
		&height,
		&channels,
		STBI_rgb_alpha // Force RGBA
	);

	if (!pixels) DX3DLogThrowError("Failed to load texture file {}", textureFileStr.c_str());
	m_texture = desc.graphicsDevice.createTexture({ {width,height}, pixels });
	m_size = { width, height };
}

const dx3d::Texture& dx3d::TextureResource::getTexture() const noexcept
{
	return *m_texture;
}

dx3d::Rect dx3d::TextureResource::getSize() const noexcept
{
	return m_size;
}
