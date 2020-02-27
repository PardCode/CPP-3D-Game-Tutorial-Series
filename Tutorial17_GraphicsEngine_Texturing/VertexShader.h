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


class VertexShader
{
public:
	VertexShader(const void* shader_byte_code, size_t byte_code_size,RenderSystem * system);
	~VertexShader();
private:
	ID3D11VertexShader * m_vs;
	RenderSystem * m_system = nullptr;
private:
	friend class RenderSystem;
	friend class DeviceContext;
};

