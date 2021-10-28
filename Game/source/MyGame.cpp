/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2021, PardCode

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

#include "MyGame.h"
#include "Spaceship.h"
#include <time.h>


MyGame::MyGame()
{
}

MyGame::~MyGame()
{
}

void MyGame::onCreate()
{
	//Adding SkyBox
	{
		auto tex = createTexture(L"Assets/Textures/stars_map.jpg");
		auto mesh = createMesh(L"Assets/Meshes/sphere.obj");
		auto mat = createMaterial(L"Assets/Shaders/skybox.hlsl");
		mat->addTexture(tex);
		mat->setCullMode(CULL_MODE_FRONT);

		auto entity = createEntity<Entity>();
		entity->setMesh(mesh);
		entity->addMaterial(mat);
		entity->setScale(Vector3D(20000, 20000, 20000));
	}

	//Adding Light
	{
		auto entity = createEntity<Light>();
		entity->setColor(Vector3D(1, 1, 1));
		entity->setRotation(Vector3D(-0.707f, 0.707f, 0));
	}

	//Adding Light
	{
		auto entity = createEntity<Light>();
		entity->setColor(Vector3D(1, 0, 0));
		entity->setRotation(Vector3D(0.707f, 0.707f, 0));
	}

	//Adding Asteroids
	{
		srand((unsigned int)time(NULL));

		auto tex = createTexture(L"Assets/Textures/asteroid.jpg");
		auto mesh = createMesh(L"Assets/Meshes/asteroid.obj");
		auto mat = createMaterial(L"Assets/Shaders/base.hlsl");
		mat->addTexture(tex);

		for (unsigned int i = 0; i < 200; i++)
		{
			auto entity = createEntity<Entity>();
			entity->setMesh(mesh);
			entity->addMaterial(mat);
			
			entity->setPosition(Vector3D((rand() % 4000) + (-2000.0f), (rand() % 4000) + (-2000.0f), (rand() % 4000) + (-2000.0f)));
			entity->setRotation(Vector3D((rand() % 628) / 100.0f, (rand() % 628) / 100.0f, (rand() % 628) / 100.0f));
			float scale = rand() % 20 + (1.0f);
			entity->setScale(Vector3D(scale, scale, scale));
		}
	}
	//Adding Spaceship
	{
		auto entity = createEntity<Spaceship>();
	}
	//Enable Play Mode - Cursor locked at the center of screen and invisible
	getInputManager()->enablePlayMode(m_input);
}


void MyGame::onUpdate(f32 deltaTime)
{
	if(getInputManager()->isKeyUp(KeyEscape))
	{
		m_input = !m_input;
		getInputManager()->enablePlayMode(m_input);
	}
}

void MyGame::onQuit()
{
}
