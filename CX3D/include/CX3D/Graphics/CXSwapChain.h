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
#include <CX3D/Math/CXRect.h>

struct CXSwapChainDesc
{
	void* windowHandle = nullptr;
	CXRect size;
};


class  CXSwapChain
{
public:
	//Initialize CXSwapChain for a window
	CXSwapChain(const CXSwapChainDesc& desc, CXGraphicsEngine* system);
	void setFullScreen(bool fullscreen, const  CXRect& size);
	void resize(const  CXRect& size);
	bool present(bool vsync);

	void* getRenderTargetView();
	void* getDepthStencilView();

private:
	void reloadBuffers(unsigned int width, unsigned int height);
private:
	Microsoft::WRL::ComPtr <IDXGISwapChain> m_swap_chain = nullptr;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> m_rtv = nullptr;
	Microsoft::WRL::ComPtr <ID3D11DepthStencilView> m_dsv = nullptr;
	CXGraphicsEngine* m_system = nullptr;
};

