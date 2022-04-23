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

#pragma once
#include <DX3D/DPrerequisites.h>
#include <DX3D/Math/DRect.h>
#include <DX3D/Resource/DTextureManager.h>
#include <DX3D/Resource/DMeshManager.h>
#include <DX3D/Resource/DMaterialManager.h>

class DGraphicsEngine
{
public:
	DGraphicsEngine();

	DTextureManager* getTextureManager();
	DMeshManager* getMeshManager();
	DMaterialManager* getMaterialManager();
public:
	DSwapChainPtr createSwapChain(void* hwnd, const DRect& size);
	DVertexBufferPtr createVertexBuffer(void* list_vertices, ui32 size_vertex, ui32 size_list);
	DIndexBufferPtr createIndexBuffer(void* list_indices, ui32 size_list);
	DConstantBufferPtr createConstantBuffer(void* buffer, ui32 size_buffer);
	DVertexShaderPtr createVertexShader(const wchar_t* file_name, const char* entry_point_name);
	DPixelShaderPtr createPixelShader(const wchar_t* file_name, const char* entry_point_name);

public:
	void clearRenderTargetColor(const DSwapChainPtr& swap_chain, float red, float green, float blue, float alpha);
	void clearDepthStencil(const DSwapChainPtr& swap_chain);

	void clearRenderTarget(const DTexturePtr& render_target, float red, float green, float blue, float alpha);
	void clearDepthStencil(const DTexturePtr& depth_stencil);

	void setRenderTarget(const DTexturePtr& render_target, const DTexturePtr& depth_stencil);

	void setVertexBuffer(const DVertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const DIndexBufferPtr& index_buffer);


	void drawTriangleList(ui32 vertex_count, ui32 start_vertex_index);
	void drawIndexedTriangleList(ui32 index_count, ui32 start_vertex_index, ui32 start_index_location);
	void drawTriangleStrip(ui32 vertex_count, ui32 start_vertex_index);

	void setViewportSize(ui32 width, ui32 height);

	void setVertexShader(const DVertexShaderPtr& vertex_shader);
	void setPixelShader(const DPixelShaderPtr& pixel_shader);

	void setTexture(const DVertexShaderPtr& vertex_shader, const DTexturePtr* texture, unsigned int num_textures);
	void setTexture(const DPixelShaderPtr& pixel_shader, const DTexturePtr* texture, unsigned int num_textures);

	void setConstantBuffer(const DVertexShaderPtr& vertex_shader, const DConstantBufferPtr& buffer);
	void setConstantBuffer(const DPixelShaderPtr& pixel_shader, const DConstantBufferPtr& buffer);
	void setMaterial(const DMaterialPtr& material);

	void setRasterizerState(bool cull_front);

	void drawMesh(const DMeshPtr& mesh, const std::vector<DMaterialPtr>& list_materials);
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

	std::unique_ptr<DTextureManager> m_texManager = nullptr;
	std::unique_ptr<DMeshManager> m_meshManager = nullptr;
	std::unique_ptr<DMaterialManager> m_materialManager = nullptr;

private:
	friend class DSwapChain;
	friend class DVertexBuffer;
	friend class DIndexBuffer;
	friend class DConstantBuffer;
	friend class DVertexShader;
	friend class DPixelShader;
	friend class DTexture;
};