// Copyright (c) 2019 - 2020 PardCode
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

#include "IndexBuffer.h"
#include "RenderSystem.h"
#include <exception>

IndexBuffer::IndexBuffer(void* list_indices, UINT size_list,RenderSystem * system) : m_system(system) , m_buffer(0)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = 4 * size_list;
	buff_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = list_indices;

	m_size_list = size_list;

	if (FAILED(m_system->m_d3d_device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
	{
		throw std::exception("IndexBuffer not created successfully");
	}
}


UINT IndexBuffer::getSizeIndexList()
{
	return this->m_size_list;
}


IndexBuffer::~IndexBuffer()
{
	m_buffer->Release();
}

