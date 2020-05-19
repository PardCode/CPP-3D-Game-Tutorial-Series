// Copyright (c) 2019 - 2020 PardCode
// All rights reserved.
//
// This file is part of CPP-3D-Game-Tutorial-Series Project, accessible from https://github.com/PardCode/CPP-3D-Game-Tutorial-Series
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License 
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

struct VS_INPUT
{
	float4 position: POSITION;
	float4 position1: POSITION1;
	float3 color: COLOR;
	float3 color1: COLOR1;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float3 color: COLOR;
	float3 color1: COLOR1;
};


cbuffer constant: register(b0)
{
	float m_angle;
};



VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	output.position = lerp(input.position, input.position1, (sin(m_angle) + 1.0f) / 2.0f);
	output.color = input.color;
	output.color1 = input.color1;
	return output;
}