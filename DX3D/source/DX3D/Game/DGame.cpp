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

#include <DX3D/Game/DGame.h>
#include <DX3D/Math/DMat4.h>
#include <Windows.h>

__declspec(align(16))
struct LightData
{
    DVec4 lightColor;
    DVec4 lightDirection;
    DVec4 lightPosition;
};


__declspec(align(16))
struct GraphicsData
{
    DMat4 world;
    DMat4 view;
    DMat4 proj;
    DVec4 cameraPosition;
    LightData lights[32];
    int numLights = 0;
};



DGame::DGame()
{
    m_inputManager = std::make_unique<DInputManager>();
    m_graphicsEngine = std::make_unique<DGraphicsEngine>();
    m_display = std::make_unique<DDisplay>(this);   

    auto rc = m_display->getClientSize();
    m_inputManager->setScreenArea(rc);
}

DGame::~DGame()
{
}

void DGame::onDisplaySize(const DRect& size)
{
    updateCamerasViewportSize();

    m_inputManager->setScreenArea(size);

    onUpdateInternal();
}

DInputManager* DGame::getInputManager()
{
    return m_inputManager.get();
}

DGraphicsEngine* DGame::getGraphicsEngine()
{
    return m_graphicsEngine.get();
}

DMaterialPtr DGame::createMaterial(const wchar_t* shader_path)
{
    return m_graphicsEngine->getMaterialManager()->createMaterialFromFile(shader_path);
}

DMaterialPtr DGame::createMaterial(const DMaterialPtr& material)
{
    return m_graphicsEngine->getMaterialManager()->createMaterial(material);
}

DTexturePtr DGame::createTexture(const wchar_t* file_path)
{
    return m_graphicsEngine->getTextureManager()->createTextureFromFile(file_path);
}

DTexturePtr DGame::createTexture(const DRect& size, const DTextureType& type)
{
    return m_graphicsEngine->getTextureManager()->createTexture(size, type);
}

DMeshPtr DGame::createMesh(const wchar_t* file_path)
{
    return m_graphicsEngine->getMeshManager()->createMeshFromFile(file_path);
}

DMeshPtr DGame::createMesh(DVertexMesh* vertex_list_data, unsigned int vertex_list_size,
    unsigned int* index_list_data, unsigned int index_list_size, 
    DMaterialSlot* material_slot_list, unsigned int material_slot_list_size)
{
    return m_graphicsEngine->getMeshManager()->createMesh(vertex_list_data, vertex_list_size,
        index_list_data, index_list_size,
        material_slot_list, material_slot_list_size);
}


void DGame::onGraphicsUpdate(f32 deltaTime)
{
    m_graphicsEngine->clearRenderTargetColor(
        m_display->getSwapChain(), 1, 0, 0, 1);

    m_graphicsEngine->setViewportSize(m_display->getClientSize().width,
        m_display->getClientSize().height);

 
    GraphicsData gdata;
   
    //processing cameras data
    {
        auto camId = typeid(DCameraEntity).hash_code();
        auto it = m_entities.find(camId);

        //let's set the camera data to the uniformdata structure, in order to pass them to the shaders for the final rendering
        if (it != m_entities.end())
        {
            for (auto& [key, camera] : it->second)
            {
                DMat4 w;
                auto cam = dynamic_cast<DCameraEntity*>(camera.get());
                cam->getViewMatrix(gdata.view);
                cam->getProjectionMatrix(gdata.proj);
                cam->getWorldMatrix(w);
                gdata.cameraPosition = w.getTranslation();
            }
        }
    }


    //processing lighting data
    {
        auto lightId = typeid(DLightEntity).hash_code();
        auto it = m_entities.find(lightId);


        //FORWARD RENDERING 
        auto i = 0;
        if (it != m_entities.end())
        {
            for (auto& [key, light] : it->second)
            {
                DMat4 w;
                auto l = dynamic_cast<DLightEntity*>(light.get());
                light->getWorldMatrix(w);
                gdata.lights[i].lightDirection = w.getZDirection();
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
            //  auto e = dynamic_cast<DMeshEntity*>(entity.get());

            if (auto e = dynamic_cast<DMeshEntity*>(entity.get()))
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

void DGame::createEntityConcrete(DEntity* entity, size_t id)
{
    auto entityPtr = std::unique_ptr<DEntity>(entity);
    auto camId = typeid(DCameraEntity).hash_code();
    entity->m_game = this;
    entity->m_id = id;
    if (id == camId)
    {
        auto it = m_entities.find(camId);
        if (it != m_entities.end())
        {
            if (it->second.size()) return;
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
}

void DGame::removeEntity(DEntity* entity)
{
   m_entitiesToDestroy.emplace(entity);
}

void DGame::updateCamerasViewportSize()
{
    {
        auto camId = typeid(DCameraEntity).hash_code();
        auto it = m_entities.find(camId);

        //let's set the camera data to the uniformdata structure, in order to pass them to the shaders for the final rendering
        if (it != m_entities.end())
        {
            for (auto& [key, entity] : it->second)
            {
                auto cam = static_cast<DCameraEntity*>(entity.get());
                cam->setScreenArea(m_display->getClientSize());
            }
        }
    }
}

void DGame::run()
{
    onCreate();

    updateCamerasViewportSize();


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
        
        onUpdateInternal();
    }

    onQuit();
}

void DGame::quit()
{
}

void DGame::setTitle(const wchar_t* title)
{
    m_display->setTitle(title);
}

void DGame::onUpdateInternal()
{
    m_inputManager->update();

    //computing delta time-------------------
    auto now = std::chrono::system_clock::now(); // take the current time
    std::chrono::duration<double> elapsedSeconds = now - m_oldTime; // let's take the difference between the current time and the time taken in the previous frame in seconds (the so called delta time)
    if (!m_oldTime.time_since_epoch().count()) //if m_oldTime has not been set yet, simply set elapsedSeconds to 0
        elapsedSeconds = std::chrono::duration<double>();
    m_oldTime = now; // store the current time in order to be used in the next frame

    f32 deltaTime = (f32)elapsedSeconds.count();
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
