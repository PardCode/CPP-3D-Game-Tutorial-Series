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

#include <CX3D/Game/CXGame.h>
#include <CX3D/Resource/CXResourceManager.h>
#include <CX3D/Math/CXMat4.h>
#include <CX3D/Resource/CXMaterial.h>
#include <CX3D/Resource/CXTexture.h>
#include <CX3D/Resource/CXMesh.h>

#include <Windows.h>

__declspec(align(16))
struct LightData
{
	CXVec4 lightColor;
	CXVec4 lightDirection;
	CXVec4 lightPosition;
};


__declspec(align(16))
struct GraphicsData
{
	CXMat4 world;
	CXMat4 view;
	CXMat4 proj;
	CXVec4 cameraPosition;
	LightData lights[32];
	int numLights = 0;
};



CXGame::CXGame()
{
	m_inputManager = std::make_unique<CXInputManager>();
	m_graphicsEngine = std::make_unique<CXGraphicsEngine>();
	m_display = std::make_unique<CXDisplay>(this);
	m_resourceManager = std::make_unique<CXResourceManager>(this);

	auto rc = m_display->getClientSize();
	m_inputManager->setScreenArea(rc);
}

CXGame::~CXGame()
{
}

void CXGame::onDisplaySize(const  CXRect& size)
{
	updateCamerasViewportSize();

	m_inputManager->setScreenArea(size);

	onUpdateInternal();
}

CXInputManager* CXGame::getInputManager()
{
	return m_inputManager.get();
}

CXGraphicsEngine* CXGame::getGraphicsEngine()
{
	return m_graphicsEngine.get();
}

CXResourceManager* CXGame::getResourceManager()
{
	return m_resourceManager.get();
}
CXMaterialPtr CXGame::createMaterial(const wchar_t* path)
{
	return m_resourceManager->createResourceFromFile<CXMaterial>(path);
}
CXTexturePtr CXGame::createTexture(const wchar_t* path)
{
	return m_resourceManager->createResourceFromFile<CXTexture>(path);
}

CXMeshPtr CXGame::createMesh(const wchar_t* path)
{
	return m_resourceManager->createResourceFromFile<CXMesh>(path);
}

void CXGame::onGraphicsUpdate(f32 deltaTime)
{
	m_graphicsEngine->clearColor(
		m_display->getSwapChain(), CXVec4(0, 0, 0, 1));

	m_graphicsEngine->setViewportSize(m_display->getClientSize().width,
		m_display->getClientSize().height);


	GraphicsData gdata;

	//processing cameras data
	{
		auto camId = typeid(CXCameraEntity).hash_code();
		auto it = m_entities.find(camId);

		//let's set the camera data to the uniformdata structure, in order to pass them to the shaders for the final rendering
		if (it != m_entities.end())
		{
			for (auto& [key, camera] : it->second)
			{
				CXMat4 w;
				auto cam = dynamic_cast<CXCameraEntity*>(camera.get());
				cam->getViewMatrix(gdata.view);
				cam->getProjectionMatrix(gdata.proj);
				cam->getWorldMatrix(w);
				gdata.cameraPosition = w.getTranslation();
			}
		}
	}


	//processing lighting data
	{
		auto lightId = typeid(CXLightEntity).hash_code();
		auto it = m_entities.find(lightId);


		//FORWARD RENDERING 
		auto i = 0;
		if (it != m_entities.end())
		{
			for (auto& [key, light] : it->second)
			{
				CXMat4 w;
				auto l = dynamic_cast<CXLightEntity*>(light.get());
				light->getWorldMatrix(w);
				gdata.lights[i].lightDirection = w.getForwardDirection();
				gdata.lights[i].lightColor = l->getColor();
				i++;
			}
			gdata.numLights = (int)it->second.size();
		}



	}


	//processing all the graphics entities
	for (auto& [key, entities] : m_entities)
	{
		//for each graphics entity
		for (auto& [key, entity] : entities)
		{
			if (auto e = dynamic_cast<CXMeshEntity*>(entity.get()))
			{
				e->getWorldMatrix(gdata.world);
				//let's retrive the world matrix and let's pass it to the uniform buffer
				for (auto mat : e->getMaterials())
				{
					mat->setData(&gdata, sizeof(GraphicsData));
				}

				m_graphicsEngine->drawMesh(e->getMesh(), e->getMaterials());
			}
			else
			{
				break;
			}
		}
	}

	m_display->getSwapChain()->present(false);

}

bool CXGame::createEntityConcrete(CXEntity* entity, size_t id)
{
	auto entityPtr = std::unique_ptr<CXEntity>(entity);
	auto camId = typeid(CXCameraEntity).hash_code();
	entity->m_game = this;
	entity->m_id = id;
	if (id == camId)
	{
		auto it = m_entities.find(camId);
		if (it != m_entities.end())
		{
			if (it->second.size()) return false;
			it->second.emplace(entity, std::move(entityPtr));
		}
		else
		{
			m_entities[camId].emplace(entity, std::move(entityPtr));
		}
	}
	else
	{
		m_entities[id].emplace(entity, std::move(entityPtr));
	}
	entity->onCreate();
	return true;
}

void CXGame::removeEntity(CXEntity* entity)
{
	m_entitiesToDestroy.emplace(entity);
}

void CXGame::updateCamerasViewportSize()
{
	{
		auto camId = typeid(CXCameraEntity).hash_code();
		auto it = m_entities.find(camId);

		//let's set the camera data to the uniformdata structure, in order to pass them to the shaders for the final rendering
		if (it != m_entities.end())
		{
			for (auto& [key, entity] : it->second)
			{
				auto cam = static_cast<CXCameraEntity*>(entity.get());
				cam->setScreenArea(m_display->getClientSize());
			}
		}
	}
}

void CXGame::run()
{
	onCreate();

	updateCamerasViewportSize();


	while (m_isRunning)
	{
		MSG msg;
		//WINDOW PROCESSING
		while (PeekMessage(&msg, HWND(), 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				m_isRunning = false;
				continue;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		onUpdateInternal();
	}

	onQuit();
}

void CXGame::quit()
{
}

void CXGame::setTitle(const wchar_t* title)
{
	std::wstring t = L"PardCode | CX3D Framework - ";
	t += title;
	m_display->setTitle(t.c_str());
}

void CXGame::onUpdateInternal()
{
	m_inputManager->update();

	//computing delta time-------------------
	auto now = std::chrono::system_clock::now(); // take the current time
	std::chrono::duration<double> elapsedSeconds = now - m_oldTime; // let's take the difference between the current time and the time taken in the previous frame in seconds (the so called delta time)
	if (!m_oldTime.time_since_epoch().count()) //if m_oldTime has not been set yet, simply set elapsedSeconds to 0
		elapsedSeconds = std::chrono::duration<double>();
	m_oldTime = now; // store the current time in order to be used in the next frame



	//computing average delta time (dt more stable)
	//WIP To fix yet.
	m_avgDt += (f32)elapsedSeconds.count();
	m_avgCount++;

	f32 deltaTime = m_avgDt / m_avgCount;

	unsigned long long max = -1;
	if (m_avgCount == 2000)
	{
		m_avgCount = 0;
		m_avgDt = (f32)elapsedSeconds.count();
	}
	//---------------------------------------------

	//destroy the entities that have been release in the previous iteration
	// we destroy the entities here in order to not create issues during the loop over the main map (m_entities)
	//--------------------------------
	for (auto entity : m_entitiesToDestroy)
	{
		auto id = entity->getId();
		m_entities[id].erase(entity);
	}
	m_entitiesToDestroy.clear();
	//----------------------------------

	//update game and entities
	//----------------------------------
	onUpdate(deltaTime);

	for (auto& [key, entities] : m_entities)
	{
		for (auto& [key, entity] : entities)
		{
			entity->onUpdate(deltaTime);
		}
	}
	//----------------------------------

	//update the graphics part
	onGraphicsUpdate(deltaTime);
}
