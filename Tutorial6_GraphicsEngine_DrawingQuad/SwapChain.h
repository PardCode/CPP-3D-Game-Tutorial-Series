#pragma once
#include <d3d11.h>

class DeviceContext;
class SwapChain
{
public:
	SwapChain();
	//Initialize SwapChain for a window
	bool init(HWND hwnd,UINT width,UINT height);


	bool present(bool vsync);

	//Release the swap chain
	bool release();
	~SwapChain();
private:
	IDXGISwapChain * m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;


private:
	friend class DeviceContext;
};

