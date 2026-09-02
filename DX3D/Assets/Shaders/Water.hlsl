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

struct WaterData
{
    float4 areaSize;
    float wavesDisplacementTexSize;
    float wavesSpeed;
};

cbuffer WaterData : register(b3)
{
    WaterData waterData;
};

Texture2D WavesDisplacement : register(t0);

VSOutput _WaterVSMain(VSInput input)
{
    VSOutput output;

    float2 texcoord = (input.texcoord * 16) + time * 0.009 * waterData.wavesSpeed;
    float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 16) + time * 0.01 * waterData.wavesSpeed;

    float height1 = WavesDisplacement.SampleLevel(DefaultSampler, texcoord, 0).r;
    float height2 = WavesDisplacement.SampleLevel(DefaultSampler, texcoord2, 0).r;

    float height = lerp(height1, height2, 0.5);

    output.position = mul(float4(
		input.position.x * waterData.areaSize.x,
		height * waterData.areaSize.y,
		input.position.z * waterData.areaSize.z,
		1),
		affineWorld);
    output.worldPosition = output.position.xyz;
    output.worldNormal = float3(0, 0, 0);

    output.position = mul(output.position, cameraData.view);
    output.position = mul(output.position, cameraData.proj);
    output.texcoord = input.texcoord;

    return output;
}


float4 _WaterPSMain(VSOutput input) : SV_TARGET
{
    float2 texcoord = (input.texcoord * 12) + time * 0.009 * waterData.wavesSpeed;
    float2 texcoord2 = (float2(-input.texcoord.x, input.texcoord.y) * 12) + time * 0.01 * waterData.wavesSpeed;

    float3 normal1 = ComputeNormalFromHeightMap(
		WavesDisplacement,
		DefaultSampler,
		waterData.wavesDisplacementTexSize,
		texcoord,
		24);

    float3 normal2 = ComputeNormalFromHeightMap(
		WavesDisplacement,
		DefaultSampler,
		waterData.wavesDisplacementTexSize,
		texcoord2,
		24);

    float3 normal = normalize(normal1 + normal2);
    normal = normalize(mul(normal, (float3x3) rigidWorld));

    float4 waterColor = float4(0.24, 0.37, 0.49, 1);
    float4 crestColor = float4(1, 1, 1, 1); 

    const float upness = abs(normal.y);
    const float minUpness = 0.7;
    const float maxUpness = 1.0;
    const float slopeBlend = smoothstep(minUpness, maxUpness, upness);
    float4 color = lerp(crestColor, waterColor, slopeBlend);

    float3 result = float3(0, 0, 0);

    //ambient light
    float ka = 3;
    float3 ia = float3(0.09, 0.09, 0.09) * color.rgb;
    float3 ambientLight = ka * ia;
    result = ambientLight;

	//directional light
    result += ComputePhongDirectionalLight(
        cameraData,
        directionLightData,
        input.worldPosition.xyz,
        normal.xyz,
        1.0, color.rgb,
        1.0, directionLightData.color.rgb,
		30.0);

    return float4(result, 0.8);
}