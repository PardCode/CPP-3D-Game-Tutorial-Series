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

#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/RenderSystem.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/VertexShader.h>
#include <DX3D/Graphics/PixelShader.h>
#include <DX3D/Resource/Material.h>
#include <DX3D/Resource/Texture.h>
#include <d3dcompiler.h>
#include <stdexcept>
#include <assert.h>


RenderSystem* Renderer = nullptr;


RenderSystem::RenderSystem()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	D3D_FEATURE_LEVEL m_feature_level;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}

	assert(SUCCEEDED(res));


	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	initRasterizerState();
	compilePrivateShaders();


	m_texManager = std::make_unique<TextureManager>();
	m_meshManager = std::make_unique<MeshManager>();
	m_materialManager = std::make_unique<MaterialManager>();

	Renderer = this;
}

TextureManager* RenderSystem::getTextureManager()
{
	return m_texManager.get();
}

MeshManager* RenderSystem::getMeshManager()
{
	return m_meshManager.get();
}

MaterialManager* RenderSystem::getMaterialManager()
{
	return m_materialManager.get();
}


RenderSystem::~RenderSystem()
{
}

SwapChainPtr RenderSystem::createSwapChain(void* hwnd, const Rect& size)
{
	return std::make_shared<SwapChain>(hwnd, size, this);
}

VertexBufferPtr RenderSystem::createVertexBuffer(void* list_vertices, UINT size_vertex, 
	UINT size_list)
{
	return std::make_shared<VertexBuffer>(list_vertices, size_vertex, size_list, this);
}

IndexBufferPtr RenderSystem::createIndexBuffer(void* list_indices, UINT size_list)
{
	return std::make_shared<IndexBuffer>(list_indices, size_list, this);
}

ConstantBufferPtr RenderSystem::createConstantBuffer(void* buffer, UINT size_buffer)
{
	return std::make_shared<ConstantBuffer>(buffer, size_buffer, this);
}


VertexShaderPtr RenderSystem::createVertexShader(const wchar_t* file_name, const char* entry_point_name)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &blob, &error_blob)))
	{
		throw std::runtime_error((char*)error_blob->GetBufferPointer());
		//if (error_blob) error_blob->Release();
		return VertexShaderPtr();
	}

	return std::make_shared<VertexShader>(blob->GetBufferPointer(), blob->GetBufferSize(), this);
}

PixelShaderPtr RenderSystem::createPixelShader(const wchar_t* file_name, const char* entry_point_name)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &blob, &error_blob)))
	{
		throw std::runtime_error((char*)error_blob->GetBufferPointer());
		return PixelShaderPtr();
	}

	return std::make_shared<PixelShader>(blob->GetBufferPointer(), blob->GetBufferSize(), this);
}


void RenderSystem::setRasterizerState(bool cull_front)
{
	if (cull_front)
		m_imm_context->RSSetState(m_cull_front_state.Get());
	else
		m_imm_context->RSSetState(m_cull_back_state.Get());
}

void RenderSystem::compilePrivateShaders()
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(L"shaders/VertexMeshLayoutShader.hlsl", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		throw std::runtime_error("Error during shaders building");
	}

	::memcpy(m_mesh_layout_byte_code, blob->GetBufferPointer(), blob->GetBufferSize());
	m_mesh_layout_size = blob->GetBufferSize();
}


void RenderSystem::clearRenderTargetColor(const SwapChainPtr& swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_imm_context->ClearRenderTargetView(swap_chain->m_rtv.Get(), clear_color);
	m_imm_context->ClearDepthStencilView(swap_chain->m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_imm_context->OMSetRenderTargets(1, swap_chain->m_rtv.GetAddressOf(), (ID3D11DepthStencilView*)swap_chain->m_dsv.Get());
}

void RenderSystem::clearDepthStencil(const SwapChainPtr& swap_chain)
{
	m_imm_context->ClearDepthStencilView(swap_chain->m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void RenderSystem::clearRenderTarget(const TexturePtr& render_target, float red, float green, float blue, float alpha)
{
	if (render_target->m_type != Texture::Type::RenderTarget) return;

	FLOAT clear_color[] = { red,green,blue,alpha };
	m_imm_context->ClearRenderTargetView(render_target->m_render_target_view.Get(), clear_color);
}

void RenderSystem::clearDepthStencil(const TexturePtr& depth_stencil)
{

	if (depth_stencil->m_type != Texture::Type::DepthStencil) return;
	m_imm_context->ClearDepthStencilView((ID3D11DepthStencilView*)depth_stencil->m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void RenderSystem::setRenderTarget(const TexturePtr& render_target, const TexturePtr& depth_stencil)
{
	if (render_target->m_type != Texture::Type::RenderTarget) return;
	if (depth_stencil->m_type != Texture::Type::DepthStencil) return;

	m_imm_context->OMSetRenderTargets(1, render_target->m_render_target_view.GetAddressOf(), (ID3D11DepthStencilView*)depth_stencil->m_depth_stencil_view.Get());
}






void RenderSystem::setVertexBuffer(const VertexBufferPtr& vertex_buffer)
{

	ui32 stride = vertex_buffer->m_size_vertex;
	ui32 offset = 0;
	m_imm_context->IASetVertexBuffers(0, 1, vertex_buffer->m_buffer.GetAddressOf(), &stride, &offset);
	m_imm_context->IASetInputLayout((ID3D11InputLayout*)vertex_buffer->m_layout.Get());
}

void RenderSystem::setIndexBuffer(const IndexBufferPtr& index_buffer)
{

	m_imm_context->IASetIndexBuffer((ID3D11Buffer*)index_buffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void RenderSystem::drawTriangleList(ui32 vertex_count, ui32 start_vertex_index)
{

	m_imm_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_imm_context->Draw(vertex_count, start_vertex_index);
}

void RenderSystem::drawIndexedTriangleList(ui32 index_count, ui32 start_vertex_index, ui32 start_index_location)
{

	m_imm_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_imm_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void RenderSystem::drawTriangleStrip(ui32 vertex_count, ui32 start_vertex_index)
{

	m_imm_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_imm_context->Draw(vertex_count, start_vertex_index);
}

void RenderSystem::setViewportSize(ui32 width, ui32 height)
{

	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_imm_context->RSSetViewports(1, &vp);
}

void RenderSystem::setVertexShader(const VertexShaderPtr& vertex_shader)
{
	m_imm_context->VSSetShader((ID3D11VertexShader*)vertex_shader->m_vs.Get(), nullptr, 0);
}

void RenderSystem::setPixelShader(const PixelShaderPtr& pixel_shader)
{
	m_imm_context->PSSetShader((ID3D11PixelShader*)pixel_shader->m_ps.Get(), nullptr, 0);
}

void RenderSystem::setTexture(const VertexShaderPtr& vertex_shader, const TexturePtr* texture, unsigned int num_textures)
{

	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = (ID3D11ShaderResourceView*)texture[i]->m_shader_res_view.Get();
		list_sampler[i] = (ID3D11SamplerState*)texture[i]->m_sampler_state.Get();
	}
	m_imm_context->VSSetShaderResources(0, num_textures, list_res);
	m_imm_context->VSSetSamplers(0, num_textures, list_sampler);
}

void RenderSystem::setTexture(const PixelShaderPtr& pixel_shader, const TexturePtr* texture, unsigned int num_textures)
{

	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = (ID3D11ShaderResourceView*)texture[i]->m_shader_res_view.Get();
		list_sampler[i] = (ID3D11SamplerState*)texture[i]->m_sampler_state.Get();
	}
	m_imm_context->PSSetShaderResources(0, num_textures, list_res);
	m_imm_context->PSSetSamplers(0, num_textures, list_sampler);
}

void RenderSystem::setConstantBuffer(const VertexShaderPtr& vertex_shader, const ConstantBufferPtr& buffer)
{

	m_imm_context->VSSetConstantBuffers(0, 1, buffer->m_buffer.GetAddressOf());
}

void RenderSystem::setConstantBuffer(const PixelShaderPtr& pixel_shader, const ConstantBufferPtr& buffer)
{

	m_imm_context->PSSetConstantBuffers(0, 1, buffer->m_buffer.GetAddressOf());
}



void RenderSystem::setMaterial(const MaterialPtr& material)
{
	 setRasterizerState((material->m_cull_mode == CULL_MODE_FRONT));

	if (material->m_constant_buffer)
	{
		  setConstantBuffer(material->m_vertex_shader, material->m_constant_buffer);
		  setConstantBuffer(material->m_pixel_shader, material->m_constant_buffer);
	}
	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	  setVertexShader(material->m_vertex_shader);
	  setPixelShader(material->m_pixel_shader);

	if (material->m_vec_textures.size())
	{
		  setTexture(material->m_pixel_shader, &material->m_vec_textures[0], (UINT)material->m_vec_textures.size());
	}
}

void RenderSystem::drawMesh(const MeshPtr& mesh, const std::vector<MaterialPtr>& list_materials)
{
	for (unsigned int m = 0; m < list_materials.size(); m++)
	{
		if (m == list_materials.size()) break;

		MaterialSlot mat = mesh->getMaterialSlot(m);

		setMaterial(list_materials[m]);
		//SET THE VERTICES OF THE TRIANGLE TO DRAW
		setVertexBuffer(mesh->getVertexBuffer());
		//SET THE INDICES OF THE TRIANGLE TO DRAW
		setIndexBuffer(mesh->getIndexBuffer());
		// FINALLY DRAW THE TRIANGLE
		drawIndexedTriangleList((UINT)mat.num_indices, 0, (UINT)mat.start_index);
	}
}


void RenderSystem::initRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;
	m_d3d_device->CreateRasterizerState(&desc, &m_cull_front_state);

	desc.CullMode = D3D11_CULL_BACK;
	m_d3d_device->CreateRasterizerState(&desc, &m_cull_back_state);
}
