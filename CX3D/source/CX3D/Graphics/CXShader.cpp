/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

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

#include <CX3D/Graphics/CXShader.h>
#include <CX3D/Graphics/CXGraphicsEngine.h>
#include <d3dcompiler.h>

CXShader::CXShader(const CXShaderDesc& desc, CXGraphicsEngine* system)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;

	if (FAILED(D3DCompileFromFile(desc.vertexShaderFilePath, nullptr, nullptr, desc.vertexShaderEntryPointName, "vs_5_0", 0, 0, &blob, &error_blob)))
		CX3D_ERROR("CXShaderProgram | Vertex Shader:" << desc.vertexShaderFilePath << " compiled with errors:" << std::endl << (char*)error_blob->GetBufferPointer());
	if (FAILED(system->m_d3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_vertexShader)))
		CX3D_ERROR("CXShaderProgram | Vertex Shader hasn't been created successfully.");


	if (FAILED(D3DCompileFromFile(desc.pixelShaderFilePath, nullptr, nullptr, desc.pixelShaderEntryPointName, "ps_5_0", 0, 0, &blob, &error_blob)))
		CX3D_ERROR("CXShaderProgram | Pixel Shader:" << desc.vertexShaderFilePath << " compiled with errors:" << std::endl << (char*)error_blob->GetBufferPointer());
	if (FAILED(system->m_d3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &m_pixelShader)))
		CX3D_ERROR("CXShaderProgram | Pixel Shader hasn't been created successfully.");
}

void* CXShader::getVertexShader()
{
	return m_vertexShader.Get();
}

void* CXShader::getPixelShader()
{
	return m_pixelShader.Get();
}
