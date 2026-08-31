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

#include <DX3D/Game/WorldRenderer.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>

#include <DX3D/Game/World.h>
#include <DX3D/Game/Component.h>
#include <DX3D/Game/GameObject.h>

#include <DX3D/Component/TransformComponent.h>
#include <DX3D/Component/CubeComponent.h>
#include <DX3D/Component/CameraComponent.h>
#include <DX3D/Component/MeshComponent.h>
#include <DX3D/Component/TerrainComponent.h>
#include <DX3D/Component/DirectionalLightComponent.h>

#include <DX3D/Resource/MaterialResource.h>
#include <DX3D/Resource/TextureResource.h>
#include <DX3D/Resource/MeshResource.h>

#include <DX3D/Math/Vec3.h>
#include <fstream>
#include <ranges>


dx3d::WorldRenderer::WorldRenderer(const WorldRendererDesc& desc): Base(desc.base), m_graphicsDevice(desc.engine)
{
	auto& device = m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	m_textures.reserve(32);

	m_objectCb = device.createConstantBuffer({ {}, sizeof(ObjectData) });
	m_cameraCb = device.createConstantBuffer({ {}, sizeof(CameraData) });
	m_envCb = device.createConstantBuffer({ {}, sizeof(EnvironmentData) });
	m_materialCb = device.createConstantBuffer({ {}, dx3d::MaterialResource::MaxDataSize });
	m_terrainCb = device.createConstantBuffer({ {}, sizeof(TerrainData) });

	m_sampler = device.createSampler({});
}

void dx3d::WorldRenderer::render(const World& world, SwapChain& swapChain, f32 deltaTime)
{
	auto size = swapChain.getSize();

	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 0.27f, 0.39f, 0.55f, 1.0f });
	//context.clearAndSetBackBuffer(swapChain, { 0,0,0,1 });
	context.setViewportSize(size);

	const Sampler* samplers[] = { m_sampler.get() };
	context.setSamplers(std::span<const Sampler*>{samplers});

	auto numComponents = 0u;

	auto& cameraCb = *m_cameraCb;
	auto& objectCb = *m_objectCb;
	auto& envCb = *m_envCb;
	auto& materialCb = *m_materialCb;


	EnvironmentData envData{};
	//directional lights
	{
		auto components = world.getComponents<dx3d::DirectionaLightComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];
			auto& transform = component->getGameObject().getTransform();
			auto dir = transform.getRigidWorldMatrix().row(2);

			envData.directionalLightData.intensity = component->getIntensity();
			envData.directionalLightData.direction = { dir.x,dir.y,dir.z };
			envData.directionalLightData.color = component->getColor();
			break;
		}
		context.updateConstantBuffer(envCb, std::as_bytes(std::span{ &envData, 1 }));
	}

	//cameras
	{	
		CameraData cameraData{};
		auto components = world.getComponents<CameraComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{		
			auto component = components[i];
			cameraData.view = component->getViewMatrix();
			component->setViewportSize(size);
			cameraData.proj = component->getProjectionMatrix();
			cameraData.position = component->getGameObject().getTransform().getPosition();
			context.updateConstantBuffer(cameraCb, std::as_bytes(std::span{ &cameraData, 1 }));
			break;
		}
	}


	//cubes
	{	
		ObjectData objectData{};
		auto components = world.getComponents<CubeComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto component = components[i];
			auto& transform = component->getGameObject().getTransform();
			
			auto material = component->getMaterial();
			
			if (material)
			{	
				objectData.affineWorld = transform.getAffineWorldMatrix();
				objectData.rigidWorld = transform.getRigidWorldMatrix();

				context.setGraphicsPipelineState(material->getGraphicsPipelineState());
				context.updateConstantBuffer(objectCb, std::as_bytes(std::span{&objectData, 1 }));
				context.updateConstantBuffer(materialCb, material->getData());
				const ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &envCb, &materialCb};
				context.setConstantBuffers(std::span<const ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t: std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<const Texture*>{m_textures});

				context.setVertexBuffer(component->getVertexBuffer());
				context.setIndexBuffer(component->getIndexBuffer());
				context.drawIndexedTriangleList(component->getIndexBuffer().getIndexListSize(), 0u, 0u);
			}
		}
	}


	//meshes
	{
		ObjectData objectData{};
		auto components = world.getComponents<MeshComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto comp = components[i];
			auto meshRes = comp->getMesh();
			if (!meshRes) continue;
			auto& mesh = *meshRes;

			objectData.affineWorld = comp->getGameObject().getTransform().getAffineWorldMatrix();
			objectData.rigidWorld = comp->getGameObject().getTransform().getRigidWorldMatrix();


			context.setVertexBuffer(mesh.getVertexBuffer());
			context.setIndexBuffer(mesh.getIndexBuffer());

			auto numSlots = 0u;
			auto slots = mesh.getMaterialSlots(numSlots);
			
			for (auto u : std::views::iota(0u, numSlots))
			{
				auto slot = slots[u];
				auto material = comp->getMaterial(u);
				if (!material) continue;
				auto numTexs = material->getNumTextures();

				context.setGraphicsPipelineState(material->getGraphicsPipelineState());
				context.updateConstantBuffer(objectCb, std::as_bytes(std::span{&objectData, 1 }));
				context.updateConstantBuffer(materialCb, material->getData());
				const ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &envCb, &materialCb};
				context.setConstantBuffers(std::span<const ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.resize(material->getNumTextures());
				for (auto t: std::views::iota(0u, m_textures.size()))
				{
					auto tex = material->getTexture(t);
					if (tex) m_textures[t] = &tex->getTexture();
				}
				context.setTextures(std::span<const Texture*>{m_textures});

				context.drawIndexedTriangleList(slot.indexCount, 0, slot.startIndex);
			}
		}
	}


	//terrains
	{
		ObjectData objectData{};
		TerrainData terrainData{};
		auto& terrainCb = *m_terrainCb;
		auto components = world.getComponents<TerrainComponent>(numComponents);
		for (auto i : std::views::iota(0u, numComponents))
		{
			auto comp = components[i];

			objectData.affineWorld = comp->getGameObject().getTransform().getAffineWorldMatrix();
			objectData.rigidWorld = comp->getGameObject().getTransform().getRigidWorldMatrix();

			terrainData.heightMapSize = static_cast<f32>(comp->getHeightMap()->getSize().width);
			auto terrainSize = comp->getSize();
			terrainData.size = { terrainSize.x,terrainSize.y,terrainSize.z, 0 };

			context.setVertexBuffer(comp->getVertexBuffer());
			context.setIndexBuffer(comp->getIndexBuffer());

			{
				context.setGraphicsPipelineState(comp->getGraphicsPipelineState());
				context.updateConstantBuffer(objectCb, std::as_bytes(std::span{ &objectData, 1 }));
				context.updateConstantBuffer(terrainCb, std::as_bytes(std::span{ &terrainData, 1 }));

				const ConstantBuffer* cbs[] = { &objectCb, &cameraCb, &envCb, &terrainCb };
				context.setConstantBuffers(std::span<const ConstantBuffer*>{cbs});

				m_textures.clear();
				m_textures.push_back(&comp->getHeightMap()->getTexture());
				m_textures.push_back(&comp->getFlatTexture()->getTexture());
				m_textures.push_back(&comp->getSlopeTexture()->getTexture());

				context.setTextures(std::span<const Texture*>{m_textures});
				context.drawIndexedTriangleList(comp->getIndexBuffer().getIndexListSize(), 0, 0);
			}
		}
	}






	m_graphicsDevice.executeCommandList(context);
	swapChain.present();
}
