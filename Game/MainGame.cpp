/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2026, PardCode

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

#include "MainGame.h"
#include "Objects/Player.h"


MainGame::MainGame(const dx3d::GameDesc& desc) : dx3d::Game(desc)
{
}

void MainGame::onCreate()
{
	Game::onCreate();
	auto& world = getWorld();

	//pedestal
	{
		auto brickTex = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/red_brick_03_diff_1k.jpg");
		auto pedestalMat = getResourceManager().createResourceFromFile<dx3d::MaterialResource>(L"Game/Assets/Shaders/MaterialShader.hlsl");
		if (pedestalMat)
		{
			float spec = 0.0f;
			pedestalMat->setData(std::as_bytes(std::span(&spec, 1)));
			pedestalMat->setTexture(0, brickTex);
		}

		auto pedestal = world.createGameObject<dx3d::GameObject>();
		pedestal->createOrGetComponent<dx3d::CubeComponent>();
		auto comp = pedestal->createOrGetComponent<dx3d::CubeComponent>();
		comp->setMaterial(pedestalMat);
		pedestal->getTransform().setScale({ 2, 2, 2 });
		pedestal->getTransform().setPosition({ 0, -1, 0 });
	}

	//marble bust
	{
		auto marbleBustTex = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/marble_bust_01_diff_1k.jpg");
		auto marbleBustMesh = getResourceManager().createResourceFromFile<dx3d::MeshResource>(L"Game/Assets/Meshes/marble_bust_01.obj");
		auto marbleBustMat = getResourceManager().createResourceFromFile<dx3d::MaterialResource>(L"Game/Assets/Shaders/MaterialShader.hlsl");
		if (marbleBustMat)
		{
			float spec = 1.0f;
			marbleBustMat->setData(std::as_bytes(std::span(&spec, 1)));
			marbleBustMat->setTexture(0, marbleBustTex);
		}
		auto mesh = world.createGameObject<dx3d::GameObject>();
		auto comp = mesh->createOrGetComponent<dx3d::MeshComponent>();
		comp->setMesh(marbleBustMesh);
		comp->setMaterial(0, marbleBustMat);
		mesh->getTransform().setScale({ 4, 4, 4 });
		mesh->getTransform().setPosition({ 0, 0, 0 });
	}

	//terrain
	{
		auto rockyTerrainTex = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/rocky_terrain_02_diff_1k.jpg");
		auto heightMap = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/Hand_made_terrain_heightmap.png");
		auto rockyTrailTex = getResourceManager().createResourceFromFile<dx3d::TextureResource>(L"Game/Assets/Textures/rocky_trail_02_diff_1k.jpg");

		auto terrain = world.createGameObject<dx3d::GameObject>();
		auto comp = terrain->createOrGetComponent<dx3d::TerrainComponent>();
		comp->setHeightMap(heightMap);	
		comp->setFlatTexture(rockyTerrainTex);
		comp->setSlopeTexture(rockyTrailTex);
	}

	//sea
	{
		auto water = world.createGameObject<dx3d::GameObject>();
		auto comp = water->createOrGetComponent<dx3d::WaterComponent>();
		water->getTransform().setPosition({ -1024,25,-1024 });
	}


	//white light
	{
		auto light = world.createGameObject<dx3d::GameObject>();
		m_whiteLight = light;
		light->createOrGetComponent<dx3d::DirectionaLightComponent>();
		auto comp = light->createOrGetComponent<dx3d::DirectionaLightComponent>();
		comp->setColor({ 1,1,1 });
		comp->setIntensity(1.09f);
		light->getTransform().setRotation({0.785f,0.785f,0 });
	}

	//player
	{
		auto player = world.createGameObject<Player>();
		player->getTransform().setPosition({150, 50, 0 });

		getInputSystem().setCursorLocked(true);
		getInputSystem().setCursorVisible(false);
	}
}


void MainGame::onUpdate(dx3d::f32 deltaTime)
{
	Game::onUpdate(deltaTime);
}
