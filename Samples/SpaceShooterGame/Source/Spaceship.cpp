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

#include "Spaceship.h"
#include "Projectile.h"


Spaceship::Spaceship()
{
}

Spaceship::~Spaceship()
{
}

void Spaceship::onCreate()
{
	//Load all the assets
	auto tex = createTexture(L"Assets/Textures/spaceship.jpg");
	auto mesh = createMesh(L"Assets/Meshes/spaceship.obj");
	auto mat = createMaterial(L"Assets/Shaders/base.hlsl");
	//Add the texture to material
	mat->addTexture(tex);

	//Set the mesh and material into the entity
	setMesh(mesh);
	addMaterial(mat);

	//Create the camera that follows the spaceship
	m_camera = getGame()->createEntity<CXCameraEntity>();
	m_camera->setFarPlane(40000.0f);
}

void Spaceship::onUpdate(f32 deltaTime)
{
	auto input = getGame()->getInputManager();

	f32 forward = 0.0f;
	f32 rightward = 0.0f;

	f32 speed = 1.0f;
	bool turbo = false;

	//Spaceship controls
	if (input->isKeyDown(CXKey::W))
	{
		forward = 1.0f;
	}
	if (input->isKeyDown(CXKey::S))
	{
		forward = -1.0f;
	}
	if (input->isKeyDown(CXKey::A))
	{
		rightward = -1.0f;
	}
	if (input->isKeyDown(CXKey::D))
	{
		rightward = 1.0f;
	}
	if (input->isKeyDown(CXKey::Shift))
	{
		speed = 3.0f;
		turbo = true;
	}

	//Handle position and rotation of spaceship and camera
	//With smooth movements, thanks to the lerp function

	if (forward)
	{
		if (turbo)
		{
			if (forward > 0.0f) m_cam_distance = 25.0f;
			else m_cam_distance = 5.0f;
		}
		else
		{
			if (forward > 0.0f) m_cam_distance = 20.0f;
			else m_cam_distance = 9.0f;
		}
	}
	else
	{
		m_cam_distance = 18.0f;
	}

	auto vec = CXVec3::lerp(CXVec3(m_current_cam_distance,0,0),
		CXVec3(m_cam_distance,0,0), 2.0f * deltaTime);
	m_current_cam_distance = vec.x;


	m_yaw += input->getMouseXAxis() * 0.001f;
	m_pitch += input->getMouseYAxis() * 0.001f;

	if (m_pitch < -1.57f)
		m_pitch = -1.57f;
	else if (m_pitch > 1.57f)
		m_pitch = 1.57f;
	
	
	auto curr = CXVec3::lerp(CXVec3(m_oldPitch, m_oldYaw, 0), CXVec3(m_pitch, m_yaw, 0), 5.0f * deltaTime);
	m_oldPitch = curr.x;
	m_oldYaw = curr.y;

	setRotation(CXVec3(m_oldPitch, m_oldYaw, 0));

	auto curr_cam = CXVec3::lerp(CXVec3(m_camPitch, m_camYaw, 0), CXVec3(m_pitch, m_yaw, 0), 3.0f * deltaTime);
	m_camPitch = curr_cam.x;
	m_camYaw = curr_cam.y;

	m_camera->setRotation(CXVec3(m_camPitch, m_camYaw, 0));


	CXMat4 w;
	getWorldMatrix(w);
	auto zdir = w.getForwardDirection();
	auto xdir = w.getRightwardDirection();
	auto ydir = w.getUpDirection();
	

	auto pos = m_position + zdir * forward * deltaTime * 100.0f * speed;
	setPosition(pos);


	CXMat4 w2;
	m_camera->getWorldMatrix(w2);
	 zdir = w2.getForwardDirection();
	 xdir = w2.getRightwardDirection();
	 ydir = w2.getUpDirection();


	auto camPos = CXVec3(pos + zdir * -m_current_cam_distance);
	camPos = camPos + ydir * 6.5f;

	m_camera->setPosition(camPos);

	//On left mouse click, spawn the projectile along the spaceship direction
	if (input->isMouseUp(CXMouseButton::Left))
	{
		auto proj = m_game->createEntity<Projectile>();
		proj->m_dir = zdir;
		proj->setPosition(pos);
	}
}
