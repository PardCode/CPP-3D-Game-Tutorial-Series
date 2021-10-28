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

#include "Spaceship.h"
#include "Projectile.h"


Spaceship::Spaceship(Game* game): Entity(game)
{
	//Load all the assets
	auto tex = m_game->createTexture(L"Assets/Textures/spaceship.jpg");
	auto mesh = m_game->createMesh(L"Assets/Meshes/spaceship.obj");
	auto mat = m_game->createMaterial(L"Assets/Shaders/base.hlsl");
	//Add the texture to material
	mat->addTexture(tex);

	//Set the mesh and material into the entity
	setMesh(mesh);
	addMaterial(mat);

	//Create the camera that follows the spaceship
	m_camera = m_game->createEntity<Camera>();
	m_camera->setFarPlane(40000.0f);
}

Spaceship::~Spaceship()
{
}

void Spaceship::onUpdate(f32 deltaTime)
{
	f32 forward = 0.0f;
	f32 rightward = 0.0f;

	f32 speed = 1.0f;
	bool turbo = false;

	//Spaceship controls
	if (m_game->getInputManager()->isKeyDown(KeyW))
	{
		forward = 1.0f;
	}
	if (m_game->getInputManager()->isKeyDown(KeyS))
	{
		forward = -1.0f;
	}
	if (m_game->getInputManager()->isKeyDown(KeyA))
	{
		rightward = -1.0f;
	}
	if (m_game->getInputManager()->isKeyDown(KeyD))
	{
		rightward = 1.0f;
	}
	if (m_game->getInputManager()->isKeyDown(KeyShift))
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

	auto vec = Vector3D::lerp(Vector3D(m_current_cam_distance,0,0),
		Vector3D(m_cam_distance,0,0), 2.0f * deltaTime);
	m_current_cam_distance = vec.x;


	m_yaw += m_game->getInputManager()->getMouseXAxis() * 0.001f;
	m_pitch += m_game->getInputManager()->getMouseYAxis() * 0.001f;

	if (m_pitch < -1.57f)
		m_pitch = -1.57f;
	else if (m_pitch > 1.57f)
		m_pitch = 1.57f;
	
	
	auto curr = Vector3D::lerp(Vector3D(m_oldPitch, m_oldYaw, 0), Vector3D(m_pitch, m_yaw, 0), 5.0f * deltaTime);
	m_oldPitch = curr.x;
	m_oldYaw = curr.y;

	setRotation(Vector3D(m_oldPitch, m_oldYaw, 0));

	auto curr_cam = Vector3D::lerp(Vector3D(m_camPitch, m_camYaw, 0), Vector3D(m_pitch, m_yaw, 0), 3.0f * deltaTime);
	m_camPitch = curr_cam.x;
	m_camYaw = curr_cam.y;

	m_camera->setRotation(Vector3D(m_camPitch, m_camYaw, 0));


	Matrix4x4 w;
	getWorldMatrix(w);
	auto zdir = w.getZDirection();
	auto xdir = w.getXDirection();
	auto ydir = w.getYDirection();
	

	auto pos = m_position + zdir * forward * deltaTime * 100.0f * speed;
	setPosition(pos);


	Matrix4x4 w2;
	m_camera->getWorldMatrix(w2);
	 zdir = w2.getZDirection();
	 xdir = w2.getXDirection();
	 ydir = w2.getYDirection();


	auto camPos = Vector3D(pos + zdir * -m_current_cam_distance);
	camPos = camPos + ydir * 6.5f;

	m_camera->setPosition(camPos);

	//On left mouse click, spawn the projectile along the spaceship direction
	if (m_game->getInputManager()->isMouseUp(MouseButtonLeft))
	{
		auto proj = m_game->createEntity<Projectile>();
		proj->m_dir = zdir;
		proj->setPosition(pos);
	}
}
