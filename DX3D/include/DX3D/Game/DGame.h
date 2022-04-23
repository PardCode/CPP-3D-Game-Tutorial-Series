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

#pragma once
#include <DX3D/DPrerequisites.h>
#include <DX3D/Game/DDisplay.h>
#include <DX3D/Graphics/DGraphicsEngine.h>
#include <DX3D/Input/DInputManager.h>
#include <DX3D/Game/DMeshEntity.h>
#include <DX3D/Game/DLightEntity.h>
#include <DX3D/Game/DCameraEntity.h>
#include <map>
#include <chrono>
#include <set>
#include <string>

class DGame
{
public:
	DGame();
	virtual ~DGame();

	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onQuit() {}

	template <typename T>
	T* createEntity()
	{
		static_assert(std::is_base_of<DEntity, T>::value, "T must derive from OEntity");
		auto id = typeid(T).hash_code();
		auto e = new T();
		createEntityConcrete(e, id);
		return e;
	}

	void setTitle(const wchar_t* title);
	void run();
	void quit();

public:
	DInputManager* getInputManager();
	DGraphicsEngine* getGraphicsEngine();

	DMaterialPtr createMaterial(const wchar_t* shader_path);
	DMaterialPtr createMaterial(const DMaterialPtr& material);
	DTexturePtr createTexture(const wchar_t* file_path);
	DTexturePtr createTexture(const DRect& size, const DTextureType& type = DTextureType::RenderTarget);

	DMeshPtr createMesh(const wchar_t* file_path);
	DMeshPtr createMesh(
		DVertexMesh* vertex_list_data, unsigned int vertex_list_size,
		unsigned int* index_list_data, unsigned int index_list_size,
		DMaterialSlot* material_slot_list, unsigned int material_slot_list_size
	);

private:	
	void onDisplaySize(const DRect& size);
	//various internal methods of entity system------------

	//function that allows to update all the data/entities etc. of the game
	void onUpdateInternal();

	//function that allows to update only the graphics part of the game
	void onGraphicsUpdate(f32 deltaTime);
	
	void createEntityConcrete(DEntity* entity, size_t id);
	void removeEntity(DEntity* entity);

	void updateCamerasViewportSize();
private:
	bool m_isRunning = true;

	std::unique_ptr<DInputManager> m_inputManager;
	std::unique_ptr<DGraphicsEngine> m_graphicsEngine;
	std::unique_ptr<DDisplay> m_display;

	//the core of entity system--------------
	//this map handles all the types of entities we add to the game.
	//for each type of entity (type retrieved with typeid function) we have a map
	//that handles the smart pointers (uniqueptr) of all the entities of that type.
	//By using unique_ptr memory management is handled easily.
	std::map<size_t, std::map<DEntity*, std::unique_ptr<DEntity>>> m_entities;

	//m_entitiesToDestroy set handles the entities that have been marked for destroy.
	std::set<DEntity*> m_entitiesToDestroy;
	//------------------------------------------

	std::chrono::system_clock::time_point m_oldTime;

	friend class DEntity;
	friend class DDisplay;
};
