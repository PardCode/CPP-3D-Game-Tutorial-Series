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
#include <DX3D/Resource/DGraphicsManager.h>
#include <DX3D/Resource/DMaterial.h>
#include <DX3D/Graphics/DConstantBuffer.h>
#include <DX3D/Graphics/DGraphicsEngine.h>
#include <stdexcept>

DMaterial::DMaterial(const wchar_t* full_path, DResourceManager* manager): DResource(full_path,manager)
{
	auto engine = static_cast<DGraphicsManager*>(m_resManager)->getGraphicsEngine();

	m_vertex_shader = engine->createVertexShader(full_path, "vsmain");
	if (!m_vertex_shader) throw std::runtime_error("DMaterial not created successfully");

	m_pixel_shader = engine->createPixelShader(full_path, "psmain");
	if (!m_pixel_shader) throw std::runtime_error("DMaterial not created successfully");
}

DMaterial::DMaterial(const DMaterialPtr & material, DResourceManager* manager) : DResource(L"", manager)
{
	m_vertex_shader = material->m_vertex_shader;
	m_pixel_shader = material->m_pixel_shader;
}


void DMaterial::addTexture(const DTexturePtr & texture)
{
	m_vec_textures.push_back(texture);
}

void DMaterial::removeTexture(unsigned int index)
{
	if (index >= this->m_vec_textures.size()) return;
	m_vec_textures.erase(m_vec_textures.begin() + index);
}

void DMaterial::setData(void * data, unsigned int size)
{
	auto engine = static_cast<DGraphicsManager*>(m_resManager)->getGraphicsEngine();

	if (!m_constant_buffer)
		m_constant_buffer = engine->createConstantBuffer(data, size);
	else
		m_constant_buffer->update(data);
}

void DMaterial::setUserData(void* data, unsigned int size)
{
	auto engine = static_cast<DGraphicsManager*>(m_resManager)->getGraphicsEngine();

	if (!m_userBuffer)
		m_userBuffer = engine->createConstantBuffer(data, size);
	else
		m_userBuffer->update(data);
}

void DMaterial::setCullMode(const DCullMode& mode)
{
	m_cull_mode = mode;
}

DCullMode DMaterial::getCullMode()
{
	return m_cull_mode;
}
