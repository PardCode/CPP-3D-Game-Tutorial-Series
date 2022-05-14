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

#include <CX3D/Resource/CXTexture.h>
#include <CX3D/Resource/CXResourceManager.h>
#include <CX3D/Graphics/CXGraphicsEngine.h>
#include <CX3D/Game/CXGame.h>

#include <DirectXTex.h>

#include <stdexcept>

CXTexture::CXTexture(const wchar_t* full_path, CXResourceManager* manager) : CXResource(full_path, manager)
{
	m_texture = manager->getGame()->getGraphicsEngine()->createTexture2D(full_path);
	if (!m_texture) CX3D_WARNING(L"CXTexture - Dynamic Texture : Creation failed");
}

CXTexture::CXTexture(const CXTexture2DDesc& desc, CXResourceManager* manager): CXResource(L"",manager)
{
	m_texture = manager->getGame()->getGraphicsEngine()->createTexture2D(desc);
	if (!m_texture) CX3D_WARNING(L"CXTexture - Dynamic Texture : Creation failed");
}

CXTexture2DPtr CXTexture::getTexture2D()
{
	return m_texture;
}

