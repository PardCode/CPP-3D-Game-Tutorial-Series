// Copyright (c) 2019  PardCode.
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#include "ConstantBuffer.h"
#include "RenderSystem.h"
#include "DeviceContext.h"
#include <exception>

ConstantBuffer::ConstantBuffer(void * buffer, UINT size_buffer,RenderSystem * system) : m_system(system)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = size_buffer;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = buffer;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("ConstantBuffer not created successfully");
	}
}


void ConstantBuffer::update(DeviceContextPtr context, void * buffer)
{
	context->m_device_context->UpdateSubresource(this->m_buffer, NULL, NULL, buffer, NULL, NULL);
}


ConstantBuffer::~ConstantBuffer()
{
	if (m_buffer)m_buffer->Release();
}
