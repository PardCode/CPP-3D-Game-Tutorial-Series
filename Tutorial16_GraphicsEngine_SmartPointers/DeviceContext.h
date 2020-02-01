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


class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context, RenderSystem * system);
	void clearRenderTargetColor(SwapChainPtr swap_chain,float red, float green, float blue, float alpha);
	void setVertexBuffer(VertexBufferPtr vertex_buffer);
	void setIndexBuffer(IndexBufferPtr index_buffer);


	void drawTriangleList(UINT vertex_count, UINT start_vertex_index);
	void drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location);
	void drawTriangleStrip(UINT vertex_count, UINT start_vertex_index);

	void setViewportSize(UINT width, UINT height);

	void setVertexShader(VertexShaderPtr vertex_shader);
	void setPixelShader(PixelShaderPtr pixel_shader);

	void setConstantBuffer(VertexShaderPtr vertex_shader, ConstantBufferPtr buffer);
	void setConstantBuffer(PixelShaderPtr pixel_shader, ConstantBufferPtr buffer);

	~DeviceContext();
private:
	ID3D11DeviceContext * m_device_context;
	RenderSystem * m_system = nullptr;
private:
	friend class ConstantBuffer;
};

