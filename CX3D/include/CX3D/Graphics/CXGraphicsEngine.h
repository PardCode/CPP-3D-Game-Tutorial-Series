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

#pragma once
#include <CX3D/CXPrerequisites.h>
#include <CX3D/Math/CXVec4.h>
#include <CX3D/Math/CXRect.h>

#include <CX3D/Graphics/CXTexture2D.h>
#include <CX3D/Graphics/CXSwapChain.h>
#include <vector>

class  CXGraphicsEngine
{
public:
	CXGraphicsEngine();
public:
	CXSwapChainPtr createSwapChain(const CXSwapChainDesc& desc);
	CXVertexBufferPtr createVertexBuffer(const CXVertexBufferDesc& desc);
	CXIndexBufferPtr createIndexBuffer(const CXIndexBufferDesc& desc);
	CXConstantBufferPtr createConstantBuffer(const CXConstantBufferDesc& desc);
	CXShaderPtr createShader(const CXShaderDesc& desc);
	CXTexture2DPtr createTexture2D(const CXTexture2DDesc& desc);
	CXTexture2DPtr createTexture2D(const wchar_t* path);
public:
	void clearColor(const  CXSwapChainPtr& swap_chain, const CXVec4& color);
	void clearDepthStencil(const  CXSwapChainPtr& swap_chain);

	void clearColor(const  CXTexture2DPtr& render_target, const CXVec4& color);
	void clearDepthStencil(const  CXTexture2DPtr& depth_stencil);

	void setRenderTarget(const  CXTexture2DPtr& render_target, const  CXTexture2DPtr& depth_stencil);

	void setVertexBuffer(const  CXVertexBufferPtr& buffer);
	void setIndexBuffer(const  CXIndexBufferPtr& buffer);


	void drawTriangleList(ui32 vertex_count, ui32 start_vertex_index);
	void drawIndexedTriangleList(ui32 index_count, ui32 start_vertex_index, ui32 start_index_location);
	void drawTriangleStrip(ui32 vertex_count, ui32 start_vertex_index);

	void setViewportSize(ui32 width, ui32 height);
	void setShader(const  CXShaderPtr& shader);
	void setConstantBuffer(const  CXConstantBufferPtr& buffer);
	void setRasterizerState(bool cull_front);


	void setTexture(const  CXTexturePtr* texture, unsigned int num_textures);
	void setMaterial(const  CXMaterialPtr& material);
	void drawMesh(const  CXMeshPtr& mesh, const std::vector<CXMaterialPtr>& list_materials);
private:
	void compilePrivateShaders();
	void initRasterizerState();

private:
	Microsoft::WRL::ComPtr < ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr < IDXGIDevice> m_dxgiDevice;
	Microsoft::WRL::ComPtr < IDXGIAdapter> m_dxgiAdapter;
	Microsoft::WRL::ComPtr < IDXGIFactory> m_dxgiFactory;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext> m_immContext;
private:
	Microsoft::WRL::ComPtr <ID3D11RasterizerState> m_cullFrontState;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState> m_cullBackState;

	unsigned char m_meshLayoutByteCode[1024];
	size_t m_meshLayoutSize = 0;

private:
	friend class  CXSwapChain;
	friend class  CXVertexBuffer;
	friend class  CXIndexBuffer;
	friend class  CXConstantBuffer;
	friend class  CXVertexShader;
	friend class  CXPixelShader;
	friend class  CXTexture2D;
	friend class  CXShader;
};