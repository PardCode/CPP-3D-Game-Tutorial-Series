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

#include <CX3D/Game/CXEntity.h>
#include <CX3D/Game/CXGame.h>

CXEntity::CXEntity()
{
}

CXEntity::~CXEntity()
{
}

void CXEntity::release()
{
	m_game->removeEntity(this);
}

void CXEntity::processWorldMatrix()
{
	CXMat4 temp;

	m_world.setIdentity();

	temp.setIdentity();
	temp.setScale(m_scale);
	m_world *= temp;


	temp.setIdentity();
	temp.setRotationX(m_rotation.x);
	m_world *= temp;

	temp.setIdentity();
	temp.setRotationY(m_rotation.y);
	m_world *= temp;

	temp.setIdentity();
	temp.setRotationZ(m_rotation.z);
	m_world *= temp;


	temp.setIdentity();
	temp.setTranslation(m_position);
	m_world *= temp;
}


size_t CXEntity::getId()
{
	return m_id;
}

void CXEntity::getWorldMatrix(CXMat4& world)
{
	world = m_world;
}

void CXEntity::setPosition(const  CXVec3& position)
{
	m_position = position;
	processWorldMatrix();
}

void CXEntity::setRotation(const  CXVec3& rotation)
{
	m_rotation = rotation;
	processWorldMatrix();
}

void CXEntity::setScale(const  CXVec3& scale)
{
	m_scale = scale;
	processWorldMatrix();
}

CXVec3 CXEntity::getPosition()
{
	return m_position;
}

CXVec3 CXEntity::getRotation()
{
	return m_rotation;
}

CXVec3 CXEntity::getScale()
{
	return m_scale;
}

CXGame* CXEntity::getGame()
{
	return m_game;
}

CXMaterialPtr CXEntity::createMaterial(const wchar_t* path)
{
	return m_game->createMaterial(path);
}

CXTexturePtr CXEntity::createTexture(const wchar_t* path)
{
	return m_game->createTexture(path);
}

CXMeshPtr CXEntity::createMesh(const wchar_t* path)
{
	return m_game->createMesh(path);
}
