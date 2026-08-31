/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2026, PardCode

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

#include "DX3D/Assets/Shaders/Common.hlsl"

struct TerrainData
{
    float4 size;
    float heightMapSize;
};

cbuffer TerrainData : register(b3)
{
    TerrainData terrainData;
};


Texture2D HeightMap : register(t0);
Texture2D FlatTexture : register(t1);
Texture2D SlopeTexture : register(t2);


VSOutput _TerrainVSMain(VSInput input)
{
    VSOutput output;

    float height = HeightMap.SampleLevel(DefaultSampler, input.texcoord, 0).r;
    output.position = mul(float4(
		input.position.x * terrainData.size.x,
		height * terrainData.size.y,
		input.position.z * terrainData.size.z,
		1),
		affineWorld);
    output.worldPosition = output.position.xyz;
    output.worldNormal = float3(0, 0, 0);

    output.position = mul(output.position, cameraData.view);
    output.position = mul(output.position, cameraData.proj);
    output.texcoord = input.texcoord;
    
    return output;
}


float4 _TerrainPSMain(VSOutput input) : SV_TARGET
{
    float3 normal = ComputeNormalFromHeightMap(
		HeightMap,
		DefaultSampler,
		terrainData.heightMapSize,
		input.texcoord,
		terrainData.size.y);

    normal = normalize(mul(normal, (float3x3) rigidWorld));

    float4 flat = FlatTexture.Sample(DefaultSampler, input.texcoord * 100.0);
    float4 slope = SlopeTexture.Sample(DefaultSampler, input.texcoord * 60.0);
    const float upness = abs(normal.y);
    const float minUpness = 0.7;
    const float maxUpness = 1.0;
    const float slopeBlend = smoothstep(minUpness, maxUpness, upness);
    float4 color = lerp(slope, flat, slopeBlend);

    float3 result = float3(0, 0, 0);
    
    //ambient light
    float ka = 0.4;
    float3 ia = float3(0.27f, 0.39f, 0.55f) * color.rgb;
    float3 ambientLight = ka * ia;
    result = ambientLight;

	//directional light
    result += ComputePhongDirectionalLight(
        cameraData,
        directionLightData,
        input.worldPosition.xyz,
        normal.xyz,
        1.0, color.rgb,
        0.0, float3(1, 1, 1),
        0.0
    );

    return float4(result, 1);
}