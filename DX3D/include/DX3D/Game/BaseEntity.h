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

#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Window/Window.h>
#include <vector>

class Game;
class BaseEntity
{
public:
	BaseEntity(Game* game);
	virtual ~BaseEntity();

public:
	void getWorldMatrix(Matrix4x4& world);
	void setPosition(const Vector3D& position);
	void setRotation(const Vector3D& rotation);
	void setScale(const Vector3D& scale);

	Vector3D getPosition();
	Vector3D getScale();


	virtual void onUpdate(f32 deltaTime) {}
protected:
	void processWorldMatrix();
protected:
	Matrix4x4 m_world;

	Vector3D m_position;
	Vector3D m_rotation;
	Vector3D m_scale = Vector3D(1,1,1);

	Game* m_game = nullptr;
};

