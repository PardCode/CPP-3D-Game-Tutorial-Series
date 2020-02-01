// Copyright (c) 2019 - 2020 PardCode
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#pragma once
#include <d3d11.h>
#include "Prerequisites.h"

class SwapChain
{
public:
	//Initialize SwapChain for a window
	SwapChain(HWND hwnd,UINT width,UINT height,RenderSystem * system);
	

	bool present(bool vsync);

	//Release the swap chain
	~SwapChain();
private:
	IDXGISwapChain * m_swap_chain;
	ID3D11RenderTargetView* m_rtv;
	ID3D11DepthStencilView* m_dsv;
	RenderSystem * m_system = nullptr;

private:
	friend class DeviceContext;
};

