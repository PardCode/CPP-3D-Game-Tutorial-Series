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

#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <stdexcept>
#include <string>

SwapChain::SwapChain(void* hwnd, const Rect& size,RenderSystem * system) : m_system(system)
{
	auto device= m_system->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BufferCount = 1;
	desc.BufferDesc.Width = (size.width>0)? size.width:1;
	desc.BufferDesc.Height = (size.height > 0) ? size.height : 1;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.OutputWindow = (HWND)hwnd;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	desc.Windowed = TRUE;

	HRESULT hr= m_system->m_dxgi_factory->CreateSwapChain(device.Get(), &desc, &m_swap_chain);
	
	if (FAILED(hr)) throw std::runtime_error("SwapChain not created successfully");

	reloadBuffers(size.width, size.height);
}


void SwapChain::setFullScreen(bool fullscreen, const Rect& size)
{
	resize(size);
	m_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

void SwapChain::resize(const Rect& size)
{
	m_rtv.Reset();
	m_dsv.Reset();

	m_swap_chain->ResizeBuffers(1, size.width, size.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	reloadBuffers(size.width, size.height);
}

bool SwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}


SwapChain::~SwapChain()
{
}

void SwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
	auto device = m_system->m_d3d_device;

	//Get the back buffer color and create its render target view
	//--------------------------------
	ID3D11Texture2D* buffer = NULL;
	HRESULT hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))throw std::runtime_error("SwapChain not created successfully");
	

	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))throw std::runtime_error("SwapChain not created successfully");
	

	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = (width > 0) ? width : 1;
	tex_desc.Height = (height > 0) ? height : 1;
	tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.MipLevels = 1;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.MiscFlags = 0;
	tex_desc.ArraySize = 1;
	tex_desc.CPUAccessFlags = 0;


	hr = device->CreateTexture2D(&tex_desc, nullptr, &buffer);
	if (FAILED(hr)) 
		throw std::runtime_error("SwapChain not created successfully");


	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);
	buffer->Release();

	if (FAILED(hr)) throw std::runtime_error("SwapChain not created successfully");
	
}
