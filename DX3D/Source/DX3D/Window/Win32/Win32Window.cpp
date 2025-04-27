/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2025, PardCode

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

#include <DX3D/Window/Window.h>
#include <Windows.h>
#include <stdexcept>

static LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}






dx3d::Window::Window(const WindowDesc& desc) : Base(desc.base), m_size(desc.size)
{
	auto registerWindowClassFunction = []()
		{
			WNDCLASSEX wc{};
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.lpszClassName = L"DX3DWindow";
			wc.lpfnWndProc = &WindowProcedure;
			return RegisterClassEx(&wc);
		};


	static const auto windowClassId = std::invoke(registerWindowClassFunction);


	if (!windowClassId)
		DX3DLogErrorAndThrow("RegisterClassEx failed.");
	
	RECT rc{ 0,0,m_size.width, m_size.height };
	AdjustWindowRect(&rc, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, false);

	m_handle = CreateWindowEx(NULL, MAKEINTATOM(windowClassId), L"PardCode | C++ 3D Game Tutorial Series",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top,
		NULL, NULL, NULL, NULL);

	if (!m_handle)
		DX3DLogErrorAndThrow("CreateWindowEx failed.");

	ShowWindow(static_cast<HWND>(m_handle), SW_SHOW);
}


dx3d::Window::~Window()
{
	DestroyWindow(static_cast<HWND>(m_handle));
}
