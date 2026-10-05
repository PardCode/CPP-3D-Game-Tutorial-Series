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

#include <DX3D/Game/WorldPhysicsSystem.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Component/TransformComponent.h>
#include <DX3D/Component/CharacterControllerComponent.h>
#include <DX3D/Component/TerrainComponent.h>

#include <ranges>


dx3d::WorldPhysicsSystem::WorldPhysicsSystem(const WorldPhysicsSystemDesc& desc): Base(desc.base)
{
}

void dx3d::WorldPhysicsSystem::update(const World& world, f32 deltaTime)
{
	auto numComponents = 0u;
	CharacterControllerComponent* characterController{};

	//characters
	{
		auto components = world.getComponents<CharacterControllerComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];
			auto& transform = component->getGameObject().getTransform();
			auto pos = transform.getPosition() + component->getMoveDirection() * component->getMoveDistance();
			transform.setPosition(pos);

			characterController = component;
			break;
		}
	}

	if (!characterController) return;
	auto& characterTransform = characterController->getGameObject().getTransform();

	//terrains
	{
		auto components = world.getComponents<TerrainComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];
			
			Vec3 hitPos{};
			if (component->findTerrainHitPosition(
				characterTransform.getPosition(),
				characterController->getMoveDirection(),
				characterController->getMoveDistance(), 
				characterController->getHeight(),
				hitPos))
			{
				characterTransform.setPosition(hitPos);
			}
		}
	}
}
