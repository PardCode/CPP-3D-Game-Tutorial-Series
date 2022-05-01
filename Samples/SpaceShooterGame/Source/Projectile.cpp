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

#include "Projectile.h"
#include "Spaceship.h"

Projectile::Projectile()
{
	
}

Projectile::~Projectile()
{
}

void Projectile::onCreate()
{
	auto mesh = m_game->createMesh(L"Assets/Meshes/sphere.obj");
	auto mat = m_game->createMaterial(L"Assets/Shaders/projectile.hlsl");

	setMesh(mesh);
	addMaterial(mat);

	setScale(CXVec3(2, 2, 2));
}

void Projectile::onUpdate(f32 deltaTime)
{
	m_elapsed += deltaTime;

	//Move the projectile along the defined direction (spaceship direction)
	auto pos = m_position + m_dir * deltaTime * 800.0f;
	setPosition(pos);
	
	//After 3 seconds, delete the projectile
	if (m_elapsed > 3.0f)
	{
		release();
	}
}
