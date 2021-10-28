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

#pragma once
#include <DX3D/Prerequisites.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Input/InputManager.h>
#include <DX3D/Game/Entity.h>
#include <DX3D/Game/Light.h>
#include <DX3D/Game/Camera.h>
#include <map>
#include <chrono>
#include <set>
#include <string>

class Game
{
public:
	Game();
	~Game();

	virtual void onCreate() {}
	virtual void onUpdate(f32 deltaTime) {}
	virtual void onQuit() {}
	void onDisplaySize(const Rect& size);

	InputManager* getInputManager();

	MaterialPtr createMaterial(const wchar_t* shader_path);
	MaterialPtr createMaterial(const MaterialPtr& material);
	TexturePtr createTexture(const wchar_t* file_path);
	TexturePtr createTexture(const Rect& size, Texture::Type type = Texture::Type::RenderTarget);

	MeshPtr createMesh(const wchar_t* file_path);
	MeshPtr createMesh(
		VertexMesh* vertex_list_data, unsigned int vertex_list_size,
		unsigned int* index_list_data, unsigned int index_list_size,
		MaterialSlot* material_slot_list, unsigned int material_slot_list_size
	);


	template <typename T>
	T* createEntity()
	{
		static_assert(std::is_base_of<BaseEntity, T>::value, "T must derive from Entity");
		auto e = new T(this);
		auto entity = std::unique_ptr<BaseEntity>(e);

		if (dynamic_cast<Camera*>(e))
		{
			if (m_cameras.size()) return nullptr;
			m_cameras.emplace(e, std::move(entity));
			onDisplaySize(m_display->getClientSize());
		}
		else if (dynamic_cast<Light*>(e))
		{
			m_lights.emplace(e, std::move(entity));
		}
		else
		{			
			m_entities.emplace(e, std::move(entity));
		}

		return (T*)e;
	}


	void removeEntity(BaseEntity* entity);
	void run();
	void quit();
private:
	void processScene();
private:
	bool m_isRunning = true;

	std::unique_ptr<InputManager> m_inputManager;
	std::unique_ptr<RenderSystem> m_renderSystem;
	std::unique_ptr<Display> m_display;
	std::map<BaseEntity*, std::unique_ptr<BaseEntity>> m_cameras;
	std::map<BaseEntity*, std::unique_ptr<BaseEntity>> m_lights;
	std::map<BaseEntity*,std::unique_ptr<BaseEntity>> m_entities;
	std::set<BaseEntity*> m_entitiesToDestroy;
	std::chrono::system_clock::time_point m_oldTime;
};
