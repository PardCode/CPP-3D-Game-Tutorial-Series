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

#pragma once

#include <CX3D/CXPrerequisites.h>
#include <CX3D/Math/CXVec2.h>
#include <CX3D/Math/CXRect.h>

class  CXInputManager
{
public:
	CXInputManager();

	virtual bool isKeyDown(const  CXKey& key);
	virtual bool isKeyUp(const  CXKey& key);

	virtual bool isMouseDown(const  CXMouseButton& button);
	virtual bool isMouseUp(const  CXMouseButton& button);

	virtual f32 getMouseXAxis();
	virtual f32 getMouseYAxis();

	virtual void enablePlayMode(bool enable);

	void setScreenArea(const  CXRect& area);
	void update();

private:
	short m_keys_state[256] = {};
	short m_old_keys_state[256] = {};
	short m_keys_state_res[256] = {};

	bool m_playEnable = false;
	CXVec2 m_old_mouse_pos;
	bool m_first_time = true;
	CXRect m_screenArea;
	CXVec2 m_deltaMouse;
	int states_index = 0;
};
