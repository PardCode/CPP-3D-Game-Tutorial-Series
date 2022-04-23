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


#include <DX3D/Window/DWindow.h>
#include <assert.h>
#include <Windows.h>
#include <stdexcept>


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	try
	{
		//GetWindowLong(hwnd,)
		switch (msg)
		{
		case WM_CREATE:
		{
			// Event fired when the window is created
			// collected here..

			break;
		}
		case WM_SIZE:
		{
			// Event fired when the window is resized
			auto window = reinterpret_cast<DWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (window) window->onSize(window->getClientSize());
			break;
		}
		case WM_SETFOCUS:
		{
			// Event fired when the window get focus
			auto window = reinterpret_cast<DWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (window) window->onFocus();
			break;
		}
		case WM_KILLFOCUS:
		{
			// Event fired when the window lost focus
			auto window = reinterpret_cast<DWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			window->onKillFocus();
			break;
		}
		case WM_DESTROY:
		{
			// Event fired when the window is destroyed
			auto window = reinterpret_cast<DWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			window->onDestroy();
			//::PostQuitMessage(0);
			break;
		}
		case WM_MOVE:
		{
			// Event fired when the window is destroyed
			auto window = reinterpret_cast<DWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
			if (window) window->onSize(window->getClientSize());
			//::PostQuitMessage(0);
			break;
		}
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			break;
		}

		default:
			return ::DefWindowProc(hwnd, msg, wparam, lparam);
		}
	}
	catch (const std::runtime_error& e)
	{
		MessageBoxA(hwnd, e.what(), "PardCode | DirectX 3D Game | Error", MB_ICONERROR | MB_OK);
		PostQuitMessage(0);
	}

	return NULL;
}


DWindow::DWindow()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"DX3DWindow";
	wc.lpfnWndProc = &WndProc;

	ATOM classId = 0;
	if (!GetClassInfoEx(HINSTANCE(), L"DX3DWindow", &wc))
	{
		classId = RegisterClassEx(&wc);
		if (!classId) throw std::runtime_error("RegisterClassEx failed");
	}
	
	RECT rc = { 0,0,1024,768 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

	m_hwnd = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		rc.right - rc.left, rc.bottom - rc.top, HWND(), HMENU(), HINSTANCE(), NULL);
	
	if (!m_hwnd) throw std::runtime_error("CreateWindowEx failed");

	SetWindowLongPtr((HWND)m_hwnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_hwnd, SW_SHOW);
	UpdateWindow((HWND)m_hwnd);
}


DRect DWindow::getClientSize()
{
	RECT rc = {};
	::GetClientRect((HWND)this->m_hwnd, &rc);
	::ClientToScreen((HWND)this->m_hwnd, (LPPOINT)&rc.left);
	::ClientToScreen((HWND)this->m_hwnd, (LPPOINT)&rc.right);
	return DRect(rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);
}

DRect DWindow::getScreenSize()
{
	RECT rc = {};

	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);

	return DRect(rc.left,rc.top,rc.right-rc.left,rc.bottom -rc.top);
}

void* DWindow::getHandle()
{
	return m_hwnd;
}

void DWindow::setTitle(const wchar_t* title)
{
	::SetWindowText((HWND)m_hwnd, title);
}

DWindow::~DWindow()
{
	DestroyWindow((HWND)m_hwnd);
}
