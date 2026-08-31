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

#include "Player.h"

Player::Player(const dx3d::GameObjectDesc& desc) : dx3d::GameObject(desc)
{
}

Player::~Player()
{
}

void Player::onCreate()
{
	createOrGetComponent<dx3d::CameraComponent>();
}

void Player::onUpdate(dx3d::f32 deltaTime)
{
	auto& input = getInputSystem();

	auto sensitivity = 0.001f;
	auto rot = getTransform().getRotation();
	rot.x += getInputSystem().getMouseDelta().y * sensitivity;
	rot.y += getInputSystem().getMouseDelta().x * sensitivity;
	if (rot.x > 1.57f) rot.x = 1.57f;
	else if (rot.x < -1.57f) rot.x = -1.57f;
	getTransform().setRotation(rot);

	auto pos = getTransform().getPosition();
	auto forward = 0.0f;
	auto right = 0.0f;
	auto speed = 50.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::W)) forward = 1.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::S)) forward = -1.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::D)) right = 1.0f;
	if (getInputSystem().isKeyDown(dx3d::KeyCode::A)) right = -1.0f;


	auto forwardDir = getTransform().forward() * forward;
	auto rightDir = getTransform().right() * right;
	auto direction = dx3d::Vec3::normalize(forwardDir + rightDir);
	pos = pos + direction * speed * deltaTime;
	getTransform().setPosition(pos);
}
