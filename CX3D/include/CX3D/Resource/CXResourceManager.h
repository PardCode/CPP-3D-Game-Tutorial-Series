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

#pragma once
#include <map>
#include <string>
#include <CX3D/CXPrerequisites.h>
#include <CX3D/Resource/CXResource.h>


class  CXResourceManager
{
public:
	CXResourceManager(CXGame* game);

	template <typename T>
	std::shared_ptr<T> createResourceFromFile(const wchar_t* path)
	{
		return std::dynamic_pointer_cast<T>(createResourceFromFileConcrete(path));
	}

	CXGame* getGame();
private:
	CXResourcePtr createResourceFromFileConcrete(const wchar_t* path);

private:
	std::map<std::wstring, CXResourcePtr> m_mapResources;
	CXGame* m_game = nullptr;
};

