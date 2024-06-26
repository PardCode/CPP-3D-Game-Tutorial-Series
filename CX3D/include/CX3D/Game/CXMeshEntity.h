/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

Copyright (c) 2019-2024, PardCode

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

#include <CX3D/Math/CXRect.h>
#include <CX3D/Math/CXMat4.h>
#include <CX3D/Math/CXVec3.h>
#include <CX3D/Game/CXEntity.h>
#include <CX3D/Window/CXWindow.h>
#include <vector>

class  CXGame;
class  CXMeshEntity : public  CXEntity
{
public:
	CXMeshEntity();
	virtual ~CXMeshEntity();
public:
	void setMesh(const  CXMeshPtr& mesh);
	void addMaterial(const  CXMaterialPtr& material);
	void removeMaterial(ui32 index);
	CXMeshPtr getMesh();
	std::vector<CXMaterialPtr>& getMaterials();
private:
	CXMeshPtr m_mesh;
	std::vector<CXMaterialPtr> m_materials;
	friend class  CXGame;
};

