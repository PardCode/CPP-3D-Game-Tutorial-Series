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

#include <CX3D/Resource/CXResourceManager.h>
#include <CX3D/Resource/CXTexture.h>
#include <CX3D/Resource/CXMesh.h>
#include <CX3D/Resource/CXTexture.h>
#include <CX3D/Resource/CXMaterial.h>


#include <filesystem>

CXResourceManager::CXResourceManager(CXGame* game):m_game(game)
{
}

CXResourceManager::~CXResourceManager()
{
}

CXResourcePtr CXResourceManager::createResourceFromFileConcrete(const wchar_t* path)
{
	auto it = m_mapResources.find(path);

	std::filesystem::path resPath = path;
	auto ext = resPath.extension();

	if (it != m_mapResources.end())
	{
		if (!ext.compare(L".hlsl") || !ext.compare(L".fx"))
		{
			return std::make_shared<CXMaterial>(std::static_pointer_cast<CXMaterial>(it->second), this);
		}
		return it->second;
	}


	if (!std::filesystem::exists(resPath)) return CXResourcePtr();

	
	if (!ext.compare(L".jpg") || !ext.compare(L".png") || !ext.compare(L".bmp") || !ext.compare(L".tga"))
	{
		//texture resource
		auto ptr = std::make_shared<CXTexture>(resPath.c_str(), this);
		if (ptr)
		{
			m_mapResources.emplace(path, ptr);
			return ptr;
		}
	}
	else if (!ext.compare(L".obj"))
	{
		//mesh resource
		auto ptr = std::make_shared<CXMesh>(resPath.c_str(), this);
		if (ptr)
		{
			m_mapResources.emplace(path, ptr);
			return ptr;
		}
	}
	else if (!ext.compare(L".hlsl") || !ext.compare(L".fx"))
	{
		//material resource
		auto ptr = std::make_shared<CXMaterial>(resPath.c_str(), this);
		if (ptr)
		{
			m_mapResources.emplace(path, ptr);
			return ptr;
		}
	}

	return CXResourcePtr();
}

CXGame* CXResourceManager::getGame()
{
	return m_game;
}
