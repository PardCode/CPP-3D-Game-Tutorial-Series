/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

Copyright (c) 2019-2024, PardCode

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
#include <CX3D/CXPrerequisites.h>
#include <CX3D/Math/CXRect.h>

#include <map>
#include <chrono>
#include <set>
#include <string>

class  CXGame
{
public:
	CXGame();
	virtual ~CXGame();

	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onQuit() {}

	template <typename T>
	T* createEntity()
	{
		static_assert(std::is_base_of<CXEntity, T>::value, "T must derive from CXEntity");
		auto id = typeid(T).hash_code();
		auto e = new T();
		if (createEntityConcrete(e, id))
			return e;
		return nullptr;
	}

	void setTitle(const wchar_t* title);
	void run();
	void quit();

public:
	CXInputManager* getInputManager();
	CXGraphicsEngine* getGraphicsEngine();
	CXResourceManager* getResourceManager();

	CXMaterialPtr createMaterial(const wchar_t* path);
	CXTexturePtr createTexture(const wchar_t* path);
	CXMeshPtr createMesh(const wchar_t* path);

private:
	void onDisplaySize(const  CXRect& size);
	//various internal methods of entity system------------

	//function that allows to update all the data/entities etc. of the game
	void onUpdateInternal();

	//function that allows to update only the graphics part of the game
	void onGraphicsUpdate(f32 deltaTime);

	bool createEntityConcrete(CXEntity* entity, size_t id);
	void removeEntity(CXEntity* entity);

	void updateCamerasViewportSize();
private:
	bool m_isRunning = true;

	std::unique_ptr<CXInputManager> m_inputManager;
	std::unique_ptr<CXGraphicsEngine> m_graphicsEngine;
	std::unique_ptr<CXDisplay> m_display; 
	std::unique_ptr<CXResourceManager> m_resourceManager;
	//the core of entity system--------------
	//this map handles all the types of entities we add to the game.
	//for each type of entity (type retrieved with typeid function) we have a map
	//that handles the smart pointers (uniqueptr) of all the entities of that type.
	//By using unique_ptr memory management is handled easily.
	std::map<size_t, std::map<CXEntity*, std::unique_ptr<CXEntity>>> m_entities;

	//m_entitiesToDestroy set handles the entities that have been marked for destroy.
	std::set<CXEntity*> m_entitiesToDestroy;
	//------------------------------------------
	f32 m_avgDt = 0.0f;
	unsigned long long m_avgCount = 0;

	std::chrono::system_clock::time_point m_oldTime;

	friend class  CXEntity;
	friend class  CXDisplay;
};
