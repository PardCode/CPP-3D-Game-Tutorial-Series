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

#include <CX3D/Graphics/CXSwapChain.h>
#include <CX3D/Graphics/CXGraphicsEngine.h>
#include <stdexcept>
#include <string>

CXSwapChain::CXSwapChain(const CXSwapChainDesc& desc, CXGraphicsEngine* system) : m_system(system)
{
	auto device = m_system->m_d3dDevice;

	DXGI_SWAP_CHAIN_DESC d3d11Desc = {};
	d3d11Desc.BufferCount = 1;
	d3d11Desc.BufferDesc.Width = (desc.size.width > 0) ? desc.size.width : 1;
	d3d11Desc.BufferDesc.Height = (desc.size.height > 0) ? desc.size.height : 1;
	d3d11Desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	d3d11Desc.BufferDesc.RefreshRate.Numerator = 60;
	d3d11Desc.BufferDesc.RefreshRate.Denominator = 1;
	d3d11Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	d3d11Desc.OutputWindow = (HWND)desc.windowHandle;
	d3d11Desc.SampleDesc.Count = 1;
	d3d11Desc.SampleDesc.Quality = 0;
	d3d11Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	d3d11Desc.Windowed = TRUE;

	HRESULT hr = m_system->m_dxgiFactory->CreateSwapChain(device.Get(), &d3d11Desc, &m_swap_chain);

	if (FAILED(hr)) throw std::runtime_error("DSwapChain not created successfully");

	reloadBuffers(desc.size.width, desc.size.height);
}


void CXSwapChain::setFullScreen(bool fullscreen, const  CXRect& size)
{
	resize(size);
	m_swap_chain->SetFullscreenState(fullscreen, nullptr);
}

void CXSwapChain::resize(const  CXRect& size)
{
	m_rtv.Reset();
	m_dsv.Reset();

	m_swap_chain->ResizeBuffers(1, size.width, size.height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	reloadBuffers(size.width, size.height);
}

bool CXSwapChain::present(bool vsync)
{
	m_swap_chain->Present(vsync, NULL);
	return true;
}

void* CXSwapChain::getRenderTargetView()
{
	return m_rtv.Get();
}

void* CXSwapChain::getDepthStencilView()
{
	return m_dsv.Get();
}

void CXSwapChain::reloadBuffers(unsigned int width, unsigned int height)
{
	auto device = m_system->m_d3dDevice;

	//Get the back buffer color and create its render target view
	//--------------------------------
	ID3D11Texture2D* buffer = NULL;
	HRESULT hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr))throw std::runtime_error("DSwapChain not created successfully");


	hr = device->CreateRenderTargetView(buffer, NULL, &m_rtv);
	buffer->Release();

	if (FAILED(hr))throw std::runtime_error("DSwapChain not created successfully");


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
		throw std::runtime_error("DSwapChain not created successfully");


	hr = device->CreateDepthStencilView(buffer, NULL, &m_dsv);
	buffer->Release();

	if (FAILED(hr)) throw std::runtime_error("DSwapChain not created successfully");

}
