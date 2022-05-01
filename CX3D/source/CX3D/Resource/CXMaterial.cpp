/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

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
#include <CX3D/Resource/CXGraphicsManager.h>
#include <CX3D/Resource/CXMaterial.h>
#include <CX3D/Graphics/CXConstantBuffer.h>
#include <CX3D/Graphics/CXGraphicsEngine.h>
#include <stdexcept>

CXMaterial::CXMaterial(const wchar_t* full_path, CXResourceManager* manager) : CXResource(full_path, manager)
{
	auto engine = static_cast<CXGraphicsManager*>(m_resManager)->getGraphicsEngine();
	m_shader = engine->createShader({ full_path, "vsmain" , full_path, "psmain" });
}

CXMaterial::CXMaterial(const  CXMaterialPtr& material, CXResourceManager* manager) : CXResource(L"", manager)
{
	m_shader = material->m_shader;
}


void CXMaterial::addTexture(const  CXTexturePtr& texture)
{
	m_vec_textures.push_back(texture);
}

void CXMaterial::removeTexture(unsigned int index)
{
	if (index >= this->m_vec_textures.size()) return;
	m_vec_textures.erase(m_vec_textures.begin() + index);
}

void CXMaterial::setData(void* data, unsigned int size)
{
	auto engine = static_cast<CXGraphicsManager*>(m_resManager)->getGraphicsEngine();

	if (!m_constant_buffer)
		m_constant_buffer = engine->createConstantBuffer({ data, size });
	else
		m_constant_buffer->update(data);
}

void CXMaterial::setUserData(void* data, unsigned int size)
{
	auto engine = static_cast<CXGraphicsManager*>(m_resManager)->getGraphicsEngine();

	if (!m_userBuffer)
		m_userBuffer = engine->createConstantBuffer({ data, size });
	else
		m_userBuffer->update(data);
}

void CXMaterial::setCullMode(const  CXCullMode& mode)
{
	m_cull_mode = mode;
}

CXCullMode CXMaterial::getCullMode()
{
	return m_cull_mode;
}
