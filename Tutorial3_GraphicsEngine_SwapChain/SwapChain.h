#pragma once
#include <d3d11.h>

class SwapChain
{
public:
	SwapChain();
	//Initialize SwapChain for a window
	bool init(HWND hwnd,UINT width,UINT height);
	//Release the swap chain
	bool release();
	~SwapChain();
private:
	IDXGISwapChain * m_swap_chain;
};

