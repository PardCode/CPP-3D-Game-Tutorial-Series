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
#include <DX3D/Math/Vector2D.h>
#include <DX3D/Math/Rect.h>

class  InputManager
{
public:
	InputManager();
	~InputManager();

	virtual bool isKeyDown(Key key);
	virtual bool isKeyUp(Key key);

	virtual bool isMouseDown(MouseButton button);
	virtual bool isMouseUp(MouseButton button);

	virtual f32 getMouseXAxis();
	virtual f32 getMouseYAxis();

	virtual void enablePlayMode(bool enable);

	void setScreenArea(const Rect& area);
	void update();

private:
	short m_keys_state[256] = {};
	short m_old_keys_state[256] = {};
	short m_keys_state_res[256] = {};

	bool m_playEnable = false;
	Vector2D m_old_mouse_pos;
	bool m_first_time = true;
	Rect m_screenArea;
	Vector2D m_deltaMouse;
	int states_index = 0;
};
