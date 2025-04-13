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

struct VS_INPUT
{
	float4 position: POSITION0;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
	float3 tangent: TANGENT0;
	float3 binormal: BINORMAL0;
};

struct VS_OUTPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};


cbuffer constant: register(b0)
{
	row_major float4x4 world;
	row_major float4x4 view;
	row_major float4x4 proj;
};



VS_OUTPUT vsmain(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//WORLD SPACE
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float3x3)world));
	return output;
}

struct PS_INPUT
{
	float4 position: SV_POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal: NORMAL0;
};

Texture2D Color: register(t0);
sampler ColorSampler: register(s0);

float4 psmain(PS_INPUT input) : SV_TARGET
{
	float4 color = Color.Sample(ColorSampler, input.texcoord);
	float3 light_direction = normalize(float3(-1, 1, 1));


	//AMBIENT LIGHT
	float ka = 8.5;
	float3 ia = float3(0.09, 0.082, 0.082);
	ia *= (color.rgb);

	float3 ambient_light = ka * ia;

	//DIFFUSE LIGHT
	float kd = 0.7;
	float amount_diffuse_light = max(dot(light_direction.xyz, input.normal),0.0);
	float3 id = float3(1, 1, 1);
	id *= (color.rgb);
	float3 diffuse_light = kd * id * amount_diffuse_light;

	//SPECULAR LIGHT
	//float ks = 0.0;
	//float3 is = float3(1.0, 1.0, 1.0);
	//float3 reflected_light = reflect(m_light_direction.xyz, input.normal);
	//float shininess = 30.0;
	//float amount_specular_light = pow(max(0.0, dot(reflected_light, input.direction_to_camera)), shininess);

	//float3 specular_light = ks * amount_specular_light * is;

	float3 final_light = ambient_light + diffuse_light /*+ specular_light*/;

	return float4(final_light,1.0);
}