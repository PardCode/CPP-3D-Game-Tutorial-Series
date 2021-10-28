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

#include <DX3D/Input/InputManager.h>
#include <Windows.h>

InputManager::InputManager()
{
}

InputManager::~InputManager()
{
}

bool InputManager::isKeyDown(Key key)
{
	short keyWin = 0;

	if (key >= KeyA && key <= KeyZ)
		keyWin = 'A' + (key - 2);
	else if (key >= Key0 && key <= Key9)
		keyWin = '0' + (key - 28);
	else if (key == KeyShift)
		keyWin = VK_SHIFT;
	else if (key == KeyEscape)
		keyWin = VK_ESCAPE;

	return  (m_keys_state_res[keyWin] == 0);
}

bool InputManager::isKeyUp(Key key)
{
	short keyWin = 0;

	if (key >= KeyA && key <= KeyZ)
		keyWin = 'A' + (key - 2);
	else if (key >= Key0 && key <= Key9)
		keyWin = '0' + (key - 28);
	else if (key == KeyShift)
		keyWin = VK_SHIFT;
	else if (key == KeyEscape)
		keyWin = VK_ESCAPE;

	return  (m_keys_state_res[keyWin] == 1);
}
	
bool InputManager::isMouseDown(MouseButton button)
{
	if (button == MouseButtonLeft)
	{
		auto state = m_keys_state_res[VK_LBUTTON];
		return (state == 0);

	}
	else if (button == MouseButtonMiddle)
	{
		auto state = m_keys_state_res[VK_MBUTTON];
		return (state == 0);
	}
	else if (button == MouseButtonRight)
	{
		auto state = m_keys_state_res[VK_RBUTTON];
		return (state == 0);
	}

	return false;
}

bool InputManager::isMouseUp(MouseButton button)
{
	if (button == MouseButtonLeft)
	{
		auto state = m_keys_state_res[VK_LBUTTON];
		return  (state == 1);

	}
	else if (button == MouseButtonMiddle)
	{
		auto state = m_keys_state_res[VK_MBUTTON];
		return  (state == 1);
	}
	else if (button == MouseButtonRight)
	{
		auto state = m_keys_state_res[VK_RBUTTON];
		return (state == 1);
	}
	return false;
}

f32 InputManager::getMouseXAxis()
{
	return m_deltaMouse.x;
}

f32 InputManager::getMouseYAxis()
{
	return m_deltaMouse.y;
}

void InputManager::enablePlayMode(bool enable)
{
	m_playEnable = enable;
	ShowCursor(!enable);
}

void InputManager::setScreenArea(const Rect& area)
{
	m_screenArea = area;
}

void InputManager::update()
{
	POINT current_mouse_pos = {};
	::GetCursorPos(&current_mouse_pos);

	if (current_mouse_pos.x != (i32)m_old_mouse_pos.x || current_mouse_pos.y != (i32)m_old_mouse_pos.y)
	{
		m_deltaMouse = Vector2D((f32)current_mouse_pos.x - (f32)m_old_mouse_pos.x, (f32)current_mouse_pos.y - (f32)m_old_mouse_pos.y);
	}
	else
	{
		m_deltaMouse = Vector2D(0, 0);
	}


	if (!m_playEnable)
	{
		m_old_mouse_pos = Vector2D((f32)current_mouse_pos.x, (f32)current_mouse_pos.y);
	}
	else
	{
		Vector2D center_screen = Vector2D(m_screenArea.left + (f32)m_screenArea.width / 2.0f, m_screenArea.top + (f32)m_screenArea.height / 2.0f);
		::SetCursorPos((i32)center_screen.x, (i32)center_screen.y);
		m_old_mouse_pos = center_screen;
	}


	for (unsigned int i = 0; i < 256; i++)
	{
		m_keys_state[i] = ::GetAsyncKeyState(i);


		if (m_keys_state[i] & 0x8001)
		{
			m_keys_state_res[i] = 0;
		}
		else // KEY IS UP
		{
			if (m_keys_state[i] != m_old_keys_state[i])
			{
				m_keys_state_res[i] = 1;
			}
			else
			{
				m_keys_state_res[i] = 2;
			}
		}
	}

	::memcpy(m_old_keys_state, m_keys_state, sizeof(short) * 256);
}
