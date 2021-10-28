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
#include <DX3D/Window/Window.h>
#include <DX3D/Math/Matrix4x4.h>
#include <DX3D/Game/BaseEntity.h>

#include <vector>

class Game;
class Camera: public BaseEntity
{
public:
	Camera(Game* game);
	~Camera();

	void getViewMatrix(Matrix4x4& view);
	void getProjectionMatrix(Matrix4x4& proj);

	void setFarPlane(f32 farPlane);
	void setScreenArea(const Rect& screen);

private:
	Matrix4x4 m_view;
	Matrix4x4 m_projection;

	f32 m_farPlane = 100.0f;
	Rect m_screenArea;
};
