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

#include <CX3D/Graphics/CXGraphicsEngine.h>
#include <CX3D/Graphics/CXSwapChain.h>
#include <CX3D/Graphics/CXGraphicsEngine.h>
#include <CX3D/Graphics/CXVertexBuffer.h>
#include <CX3D/Graphics/CXIndexBuffer.h>
#include <CX3D/Graphics/CXConstantBuffer.h>
#include <CX3D/Graphics/CXVertexShader.h>
#include <CX3D/Graphics/CXPixelShader.h>
#include <CX3D/Resource/CXMaterial.h>
#include <CX3D/Resource/CXTexture.h>
#include <d3dcompiler.h>
#include <stdexcept>
#include <assert.h>


CXGraphicsEngine::CXGraphicsEngine()
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
			num_feature_levels, D3D11_SDK_VERSION, &m_d3dDevice, &m_feature_level, &m_immContext);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}


	if (FAILED(res))
		throw std::runtime_error("DGraphicsEngine not created successfully");



	m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
	m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
	m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

	initRasterizerState();
	compilePrivateShaders();


	m_texManager = std::make_unique<CXTextureManager>(this);
	m_meshManager = std::make_unique<CXMeshManager>(this);
	m_materialManager = std::make_unique<CXMaterialManager>(this);
}

CXTextureManager* CXGraphicsEngine::getTextureManager()
{
	return m_texManager.get();
}

CXMeshManager* CXGraphicsEngine::getMeshManager()
{
	return m_meshManager.get();
}

CXMaterialManager* CXGraphicsEngine::getMaterialManager()
{
	return m_materialManager.get();
}

CXSwapChainPtr CXGraphicsEngine::createSwapChain(void* hwnd, const  CXRect& size)
{
	return std::make_shared<CXSwapChain>(hwnd, size, this);
}

CXVertexBufferPtr CXGraphicsEngine::createVertexBuffer(void* list_vertices, UINT size_vertex,
	UINT size_list)
{
	return std::make_shared<CXVertexBuffer>(list_vertices, size_vertex, size_list, this);
}

CXIndexBufferPtr CXGraphicsEngine::createIndexBuffer(void* list_indices, UINT size_list)
{
	return std::make_shared<CXIndexBuffer>(list_indices, size_list, this);
}

CXConstantBufferPtr CXGraphicsEngine::createConstantBuffer(void* buffer, UINT size_buffer)
{
	return std::make_shared<CXConstantBuffer>(buffer, size_buffer, this);
}


CXVertexShaderPtr CXGraphicsEngine::createVertexShader(const wchar_t* file_name, const char* entry_point_name)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &blob, &error_blob)))
	{
		throw std::runtime_error((char*)error_blob->GetBufferPointer());
		//if (error_blob) error_blob->Release();
		return CXVertexShaderPtr();
	}

	return std::make_shared<CXVertexShader>(blob->GetBufferPointer(), blob->GetBufferSize(), this);
}

CXPixelShaderPtr CXGraphicsEngine::createPixelShader(const wchar_t* file_name, const char* entry_point_name)
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &blob, &error_blob)))
	{
		throw std::runtime_error((char*)error_blob->GetBufferPointer());
		return CXPixelShaderPtr();
	}

	return std::make_shared<CXPixelShader>(blob->GetBufferPointer(), blob->GetBufferSize(), this);
}


void CXGraphicsEngine::setRasterizerState(bool cull_front)
{
	if (cull_front)
		m_immContext->RSSetState(m_cullFrontState.Get());
	else
		m_immContext->RSSetState(m_cullBackState.Get());
}

void CXGraphicsEngine::compilePrivateShaders()
{
	ID3DBlob* blob = nullptr;
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(L"Engine/Shaders/VertexMeshLayoutShader.hlsl", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		throw std::runtime_error("Error during shaders building");
	}

	::memcpy(m_meshLayoutByteCode, blob->GetBufferPointer(), blob->GetBufferSize());
	m_meshLayoutSize = blob->GetBufferSize();
}


void CXGraphicsEngine::clearRenderTargetColor(const  CXSwapChainPtr& swap_chain, const CXVec4& color)
{
	FLOAT clear_color[] = { color.x, color.y, color.z, color.w };
	m_immContext->ClearRenderTargetView(swap_chain->m_rtv.Get(), clear_color);
	m_immContext->ClearDepthStencilView(swap_chain->m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_immContext->OMSetRenderTargets(1, swap_chain->m_rtv.GetAddressOf(), (ID3D11DepthStencilView*)swap_chain->m_dsv.Get());
}

void CXGraphicsEngine::clearDepthStencil(const  CXSwapChainPtr& swap_chain)
{
	m_immContext->ClearDepthStencilView(swap_chain->m_dsv.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void CXGraphicsEngine::clearRenderTarget(const  CXTexturePtr& render_target, const CXVec4& color)
{
	if (render_target->m_type != CXTextureType::RenderTarget) return;

	FLOAT clear_color[] = { color.x, color.y, color.z, color.w };
	m_immContext->ClearRenderTargetView(render_target->m_render_target_view.Get(), clear_color);
}

void CXGraphicsEngine::clearDepthStencil(const  CXTexturePtr& depth_stencil)
{

	if (depth_stencil->m_type != CXTextureType::DepthStencil) return;
	m_immContext->ClearDepthStencilView((ID3D11DepthStencilView*)depth_stencil->m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void CXGraphicsEngine::setRenderTarget(const  CXTexturePtr& render_target, const  CXTexturePtr& depth_stencil)
{
	if (render_target->m_type != CXTextureType::RenderTarget) return;
	if (depth_stencil->m_type != CXTextureType::DepthStencil) return;

	m_immContext->OMSetRenderTargets(1, render_target->m_render_target_view.GetAddressOf(), (ID3D11DepthStencilView*)depth_stencil->m_depth_stencil_view.Get());
}






void CXGraphicsEngine::setVertexBuffer(const  CXVertexBufferPtr& vertex_buffer)
{

	ui32 stride = vertex_buffer->m_size_vertex;
	ui32 offset = 0;
	m_immContext->IASetVertexBuffers(0, 1, vertex_buffer->m_buffer.GetAddressOf(), &stride, &offset);
	m_immContext->IASetInputLayout((ID3D11InputLayout*)vertex_buffer->m_layout.Get());
}

void CXGraphicsEngine::setIndexBuffer(const  CXIndexBufferPtr& index_buffer)
{

	m_immContext->IASetIndexBuffer((ID3D11Buffer*)index_buffer->m_buffer.Get(), DXGI_FORMAT_R32_UINT, 0);
}

void CXGraphicsEngine::drawTriangleList(ui32 vertex_count, ui32 start_vertex_index)
{

	m_immContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_immContext->Draw(vertex_count, start_vertex_index);
}

void CXGraphicsEngine::drawIndexedTriangleList(ui32 index_count, ui32 start_vertex_index, ui32 start_index_location)
{

	m_immContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_immContext->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void CXGraphicsEngine::drawTriangleStrip(ui32 vertex_count, ui32 start_vertex_index)
{

	m_immContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_immContext->Draw(vertex_count, start_vertex_index);
}

void CXGraphicsEngine::setViewportSize(ui32 width, ui32 height)
{

	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_immContext->RSSetViewports(1, &vp);
}

void CXGraphicsEngine::setVertexShader(const  CXVertexShaderPtr& vertex_shader)
{
	m_immContext->VSSetShader((ID3D11VertexShader*)vertex_shader->m_vs.Get(), nullptr, 0);
}

void CXGraphicsEngine::setPixelShader(const  CXPixelShaderPtr& pixel_shader)
{
	m_immContext->PSSetShader((ID3D11PixelShader*)pixel_shader->m_ps.Get(), nullptr, 0);
}

void CXGraphicsEngine::setTexture(const  CXVertexShaderPtr& vertex_shader, const  CXTexturePtr* texture, unsigned int num_textures)
{

	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = (ID3D11ShaderResourceView*)texture[i]->m_shader_res_view.Get();
		list_sampler[i] = (ID3D11SamplerState*)texture[i]->m_sampler_state.Get();
	}
	m_immContext->VSSetShaderResources(0, num_textures, list_res);
	m_immContext->VSSetSamplers(0, num_textures, list_sampler);
}

void CXGraphicsEngine::setTexture(const  CXPixelShaderPtr& pixel_shader, const  CXTexturePtr* texture, unsigned int num_textures)
{

	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = (ID3D11ShaderResourceView*)texture[i]->m_shader_res_view.Get();
		list_sampler[i] = (ID3D11SamplerState*)texture[i]->m_sampler_state.Get();
	}
	m_immContext->PSSetShaderResources(0, num_textures, list_res);
	m_immContext->PSSetSamplers(0, num_textures, list_sampler);
}

void CXGraphicsEngine::setConstantBuffer(const  CXVertexShaderPtr& vertex_shader, const  CXConstantBufferPtr& buffer)
{

	m_immContext->VSSetConstantBuffers(0, 1, buffer->m_buffer.GetAddressOf());
}

void CXGraphicsEngine::setConstantBuffer(const  CXPixelShaderPtr& pixel_shader, const  CXConstantBufferPtr& buffer)
{

	m_immContext->PSSetConstantBuffers(0, 1, buffer->m_buffer.GetAddressOf());
}



void CXGraphicsEngine::setMaterial(const  CXMaterialPtr& material)
{
	setRasterizerState((material->m_cull_mode == CXCullMode::Front));

	if (material->m_constant_buffer)
	{
		setConstantBuffer(material->m_vertex_shader, material->m_constant_buffer);
		setConstantBuffer(material->m_pixel_shader, material->m_constant_buffer);
	}
	//SET CXEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO CXRAW
	setVertexShader(material->m_vertex_shader);
	setPixelShader(material->m_pixel_shader);

	if (material->m_vec_textures.size())
	{
		setTexture(material->m_pixel_shader, &material->m_vec_textures[0], (UINT)material->m_vec_textures.size());
	}
}

void CXGraphicsEngine::drawMesh(const  CXMeshPtr& mesh, const std::vector<CXMaterialPtr>& list_materials)
{
	for (unsigned int m = 0; m < list_materials.size(); m++)
	{
		if (m == list_materials.size()) break;

		CXMaterialSlot mat = mesh->getMaterialSlot(m);

		setMaterial(list_materials[m]);
		//SET THE VERTICES OF THE TRIANGLE TO CXRAW
		setVertexBuffer(mesh->getVertexBuffer());
		//SET THE INDICES OF THE TRIANGLE TO CXRAW
		setIndexBuffer(mesh->getIndexBuffer());
		// FINALLY CXRAW THE TRIANGLE
		drawIndexedTriangleList((UINT)mat.num_indices, 0, (UINT)mat.start_index);
	}
}


void CXGraphicsEngine::initRasterizerState()
{
	D3D11_RASTERIZER_DESC desc = {};
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = true;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.FrontCounterClockwise = true;
	m_d3dDevice->CreateRasterizerState(&desc, &m_cullFrontState);

	desc.CullMode = D3D11_CULL_BACK;
	m_d3dDevice->CreateRasterizerState(&desc, &m_cullBackState);
}
