/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2021, PardCode

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
#include <DX3D/Prerequisites.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Resource/TextureManager.h>
#include <DX3D/Resource/MeshManager.h>
#include <DX3D/Resource/MaterialManager.h>

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	TextureManager* getTextureManager();
	MeshManager* getMeshManager();
	MaterialManager* getMaterialManager();
public:
	SwapChainPtr createSwapChain(void* hwnd, const Rect& size);
	VertexBufferPtr createVertexBuffer(void* list_vertices, ui32 size_vertex, ui32 size_list);
	IndexBufferPtr createIndexBuffer(void* list_indices, ui32 size_list);
	ConstantBufferPtr createConstantBuffer(void* buffer, ui32 size_buffer);
	VertexShaderPtr createVertexShader(const wchar_t* file_name, const char* entry_point_name);
	PixelShaderPtr createPixelShader(const wchar_t* file_name, const char* entry_point_name);

public:
	void clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha);
	void clearDepthStencil(const SwapChainPtr& swap_chain);

	void clearRenderTarget(const TexturePtr& render_target, float red, float green, float blue, float alpha);
	void clearDepthStencil(const TexturePtr& depth_stencil);

	void setRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil);

	void setVertexBuffer(const VertexBufferPtr& vertex_buffer);
	void setIndexBuffer(const IndexBufferPtr& index_buffer);


	void drawTriangleList(ui32 vertex_count, ui32 start_vertex_index);
	void drawIndexedTriangleList(ui32 index_count, ui32 start_vertex_index, ui32 start_index_location);
	void drawTriangleStrip(ui32 vertex_count, ui32 start_vertex_index);

	void setViewportSize(ui32 width, ui32 height);

	void setVertexShader(const VertexShaderPtr& vertex_shader);
	void setPixelShader(const PixelShaderPtr& pixel_shader);

	void setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures);
	void setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures);

	void setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer);
	void setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer);
	void setMaterial(const MaterialPtr& material);

	void setRasterizerState(bool cull_front);

	void drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials);
private:
	void compilePrivateShaders();
	void initRasterizerState();

private:
	Microsoft::WRL::ComPtr < ID3D11Device> m_d3d_device;
	Microsoft::WRL::ComPtr < IDXGIDevice> m_dxgi_device;
	Microsoft::WRL::ComPtr < IDXGIAdapter> m_dxgi_adapter;
	Microsoft::WRL::ComPtr < IDXGIFactory> m_dxgi_factory;
	Microsoft::WRL::ComPtr < ID3D11DeviceContext> m_imm_context;
private:
	Microsoft::WRL::ComPtr <ID3D11RasterizerState> m_cull_front_state;
	Microsoft::WRL::ComPtr <ID3D11RasterizerState> m_cull_back_state;

	unsigned char m_mesh_layout_byte_code[1024];
	size_t m_mesh_layout_size = 0;

	std::unique_ptr<TextureManager> m_texManager = nullptr;
	std::unique_ptr<MeshManager> m_meshManager = nullptr;
	std::unique_ptr<MaterialManager> m_materialManager = nullptr;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;
	friend class Texture;
};