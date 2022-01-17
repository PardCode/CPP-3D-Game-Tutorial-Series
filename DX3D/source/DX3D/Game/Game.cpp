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

#include <DX3D/Game/Game.h>
#include <DX3D/Math/Matrix4x4.h>
#include <Windows.h>

__declspec(align(16))
struct light
{
    Vector4D m_lightColor;
    Vector4D m_lightDirection;
    Vector4D m_lightPosition;
};


__declspec(align(16))
struct constant
{
    Matrix4x4 world;
    Matrix4x4 m_view;
    Matrix4x4 m_proj;
    Vector4D m_camera_position;
    light m_lights[32];
    int m_numLights = 0;
};



Game::Game()
{
    m_inputManager = std::make_unique<InputManager>();
    m_renderSystem = std::make_unique<RenderSystem>();
    m_display = std::make_unique<Display>(this);   

    m_display->setTitle(L"PardCode | DirectX 3D Game");

    auto rc = m_display->getClientSize();

    for (auto &[key,camera] : m_cameras)
        dynamic_cast<Camera*>(camera.get())->setScreenArea(rc);
    

    m_inputManager->setScreenArea(rc);
}

Game::~Game()
{
}

void Game::onDisplaySize(const Rect& size)
{
    for (auto& [key, camera] : m_cameras)
        dynamic_cast<Camera*>(camera.get())->setScreenArea(size);
    

    m_inputManager->setScreenArea(size);


    processScene();
}

InputManager* Game::getInputManager()
{
    return m_inputManager.get();
}

MaterialPtr Game::createMaterial(const wchar_t* shader_path)
{
    return m_renderSystem->getMaterialManager()->createMaterialFromFile(shader_path);
}

MaterialPtr Game::createMaterial(const MaterialPtr& material)
{
    return m_renderSystem->getMaterialManager()->createMaterial(material);
}

TexturePtr Game::createTexture(const wchar_t* file_path)
{
    return m_renderSystem->getTextureManager()->createTextureFromFile(file_path);
}

TexturePtr Game::createTexture(const Rect& size, Texture::Type type)
{
    return m_renderSystem->getTextureManager()->createTexture(size, type);
}

MeshPtr Game::createMesh(const wchar_t* file_path)
{
    return m_renderSystem->getMeshManager()->createMeshFromFile(file_path);
}

MeshPtr Game::createMesh(VertexMesh* vertex_list_data, unsigned int vertex_list_size, 
    unsigned int* index_list_data, unsigned int index_list_size, 
    MaterialSlot* material_slot_list, unsigned int material_slot_list_size)
{
    return m_renderSystem->getMeshManager()->createMesh(vertex_list_data, vertex_list_size,
        index_list_data, index_list_size,
        material_slot_list, material_slot_list_size);
}


void Game::removeEntity(BaseEntity* entity)
{
   m_entitiesToDestroy.emplace(entity);
}

void Game::run()
{
    onCreate();
    while (m_isRunning)
    {
        MSG msg;
        //WINDOW PROCESSING
        if (PeekMessage(&msg, HWND(), 0, 0, PM_REMOVE))
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
        
        processScene();
    }

    onQuit();
}

void Game::quit()
{
}

void Game::processScene()
{

    auto now = std::chrono::system_clock::now();
    auto diff = now - m_oldTime;
    std::chrono::duration<double> elapsedSeconds = now - m_oldTime;
    if (!m_oldTime.time_since_epoch().count())
        elapsedSeconds = std::chrono::duration<double>();
    m_oldTime = now;

    //ERASE PENDING REMOVED ENTITIES
    for (auto entity : m_entitiesToDestroy)
    {
        m_entities.erase(entity);
        m_cameras.erase(entity);
        m_lights.erase(entity);
    }
    m_entitiesToDestroy.clear();

    //UPDATE
    {
        onUpdate((f32)elapsedSeconds.count());

        for (auto& [key, entity] : m_cameras)
        {
            entity->onUpdate((f32)elapsedSeconds.count());
        }

        for (auto& [key, entity] : m_lights)
        {
            entity->onUpdate((f32)elapsedSeconds.count());
        }

        for (auto& [key, entity] : m_entities)
        {
            entity->onUpdate((f32)elapsedSeconds.count());
        }
    }

    //SIMPLE RENDERING LOOP
    {
        m_renderSystem->clearRenderTargetColor(
            m_display->getSwapChain(), 1, 0, 0, 1);

        m_renderSystem->setViewportSize(m_display->getClientSize().width,
            m_display->getClientSize().height);

        constant cc;

        for (auto& [key, camera] : m_cameras)
        {
            Matrix4x4 w;
            auto cam = dynamic_cast<Camera*>(camera.get());
            cam->getViewMatrix(cc.m_view);
            cam->getProjectionMatrix(cc.m_proj);
            cam->getWorldMatrix(w);
            cc.m_camera_position = w.getTranslation();
        }


        //FORWARD RENDERING 
        auto i = 0;
        for (auto& [key, light] : m_lights)
        {
            Matrix4x4 w;
            auto l = dynamic_cast<Light*>(light.get());
            light->getWorldMatrix(w);
            cc.m_lights[i].m_lightDirection = w.getZDirection();
            cc.m_lights[i].m_lightColor = l->getColor();
            i++;
        }

        cc.m_numLights = (int)m_lights.size();

        for (auto& [key, entity] : m_entities)
        {
            auto e = dynamic_cast<Entity*>(entity.get());

            if (e)
            {
                e->getWorldMatrix(cc.world);

                for (auto mat : e->getMaterial())
                {
                    mat->setData(&cc, sizeof(constant));
                }

                m_renderSystem->drawMesh(e->getMesh(), e->getMaterial());
            }
        }


        m_display->getSwapChain()->present(false);
    }

    //INPUT
    m_inputManager->update();
}
