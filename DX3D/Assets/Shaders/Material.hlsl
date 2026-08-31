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

struct MaterialVSOut
{
};

struct MaterialPSOut
{
    float4 diffuse;
    float4 specular;
    float shininess;
};

void VSMain(inout MaterialVSOut output);
void PSMain(inout MaterialPSOut output);

static float2 TextureCoordinate = float2(0, 0);

VSOutput _VSMain(VSInput input)
{
    VSOutput output;
    output.position = mul(float4(input.position, 1), affineWorld);
    output.worldPosition = output.position.xyz;
    output.worldNormal = normalize(mul(input.normal, (float3x3) rigidWorld));

    output.position = mul(output.position, cameraData.view);
    output.position = mul(output.position, cameraData.proj);
    output.texcoord = input.texcoord;

    TextureCoordinate = input.texcoord;
    MaterialVSOut vsOut;
    VSMain(vsOut);
    
    return output;
}

float4 _PSMain(VSOutput input) : SV_TARGET
{
    
    TextureCoordinate = input.texcoord;
    MaterialPSOut psOut;
    psOut.diffuse = float4(1, 1, 1, 1);
    psOut.specular = float4(0, 0, 0, 0);
    psOut.shininess = 0.0;
    PSMain(psOut);

    
    float3 result = float3(0, 0, 0);

	//ambient light
    float ka = 0.1;
    float3 ia = float3(0.27f, 0.39f, 0.55f) * psOut.diffuse.rgb;
    float3 ambientLight = ka * ia;
    result = ambientLight;

	//directional light
    result += ComputePhongDirectionalLight(
        cameraData,
        directionLightData,
        input.worldPosition,
        input.worldNormal,
        1.0, psOut.diffuse.rgb,
        1.0, psOut.specular.rgb,
        psOut.shininess
    );

    return float4(result, 1);
}