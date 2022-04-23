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

#include <DX3D/Game/DEntity.h>
#include <DX3D/Game/DGame.h>

DEntity::DEntity()
{
}

DEntity::~DEntity()
{
}

void DEntity::release()
{
	m_game->removeEntity(this);
}

void DEntity::processWorldMatrix()
{
		DMat4 temp;

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


size_t DEntity::getId()
{
	return m_id;
}

void DEntity::getWorldMatrix(DMat4& world)
{
	world = m_world;
}

void DEntity::setPosition(const DVec3& position)
{
	m_position = position;
	processWorldMatrix();
}

void DEntity::setRotation(const DVec3& rotation)
{
	m_rotation = rotation;
	processWorldMatrix();
}

void DEntity::setScale(const DVec3& scale)
{
	m_scale = scale;
	processWorldMatrix();
}

DVec3 DEntity::getPosition()
{
	return m_position;
}

DVec3 DEntity::getScale()
{
	return m_scale;
}
