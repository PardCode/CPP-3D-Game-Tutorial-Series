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

#include <CX3D/Math/CXRect.h>
#include <CX3D/Math/CXMat4.h>
#include <CX3D/Math/CXVec3.h>
#include <vector>

class  CXGame;
class  CXEntity
{
public:
	CXEntity();
	virtual ~CXEntity();

public:
	size_t getId();
	void getWorldMatrix(CXMat4& world);
	void setPosition(const  CXVec3& position);
	void setRotation(const  CXVec3& rotation);
	void setScale(const  CXVec3& scale);

	CXVec3 getPosition();
	CXVec3 getRotation();
	CXVec3 getScale();

	CXGame* getGame();

	CXMaterialPtr createMaterial(const wchar_t* path);
	CXTexturePtr createTexture(const wchar_t* path);
	CXMeshPtr createMesh(const wchar_t* path);


	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void release();
protected:
	void processWorldMatrix();
protected:
	CXMat4 m_world;

	CXVec3 m_position;
	CXVec3 m_rotation;
	CXVec3 m_scale = CXVec3(1, 1, 1);

	size_t m_id = 0;

	CXGame* m_game = nullptr;

	friend class  CXGame;
};

