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
#include <CX3D/Graphics/CXShader.h>
#include <CX3D/Resource/CXMaterial.h>
#include <CX3D/Resource/CXTexture.h>
#include <CX3D/Resource/CXMesh.h>

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
}

CXSwapChainPtr CXGraphicsEngine::createSwapChain(const CXSwapChainDesc& desc)
{
	return std::make_shared<CXSwapChain>(desc, this);
}

CXVertexBufferPtr CXGraphicsEngine::createVertexBuffer(const CXVertexBufferDesc& desc)
{
	return std::make_shared<CXVertexBuffer>(desc, this);
}

CXIndexBufferPtr CXGraphicsEngine::createIndexBuffer(const CXIndexBufferDesc& desc)
{
	return std::make_shared<CXIndexBuffer>(desc, this);
}

CXConstantBufferPtr CXGraphicsEngine::createConstantBuffer(const CXConstantBufferDesc& desc)
{
	return std::make_shared<CXConstantBuffer>(desc, this);
}

CXShaderPtr CXGraphicsEngine::createShader(const CXShaderDesc& desc)
{
	return std::make_shared<CXShader>(desc, this);
}

CXTexture2DPtr CXGraphicsEngine::createTexture2D(const CXTexture2DDesc& desc)
{
	return std::make_shared<CXTexture2D>(desc, this);
}

CXTexture2DPtr CXGraphicsEngine::createTexture2D(const wchar_t* path)
{
	return std::make_shared<CXTexture2D>(path, this);
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

	auto layoutCode = R"(
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
			};

			VS_OUTPUT vsmain(VS_INPUT input)
			{
				VS_OUTPUT output = (VS_OUTPUT)0;
				return output;
			}
		)";

	auto codeLength = strlen(layoutCode);
	if (FAILED(D3DCompile(layoutCode, codeLength,"VertexMeshLayoutShader", nullptr, nullptr, "vsmain", "vs_5_0", 0, 0, &blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		CX3D_ERROR("VertexMeshLayoutShader compiled with errors.");
	}

	::memcpy(m_meshLayoutByteCode, blob->GetBufferPointer(), blob->GetBufferSize());
	m_meshLayoutSize = blob->GetBufferSize();
}


void CXGraphicsEngine::clearColor(const  CXSwapChainPtr& swap_chain, const CXVec4& color)
{
	FLOAT clear_color[] = { color.x, color.y, color.z, color.w };
	auto rtv = reinterpret_cast<ID3D11RenderTargetView*>(swap_chain->getRenderTargetView());
	auto dsv = reinterpret_cast<ID3D11DepthStencilView*>(swap_chain->getDepthStencilView());

	m_immContext->ClearRenderTargetView(rtv, clear_color);
	m_immContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_immContext->OMSetRenderTargets(1, &rtv, dsv);
}

void CXGraphicsEngine::clearDepthStencil(const  CXSwapChainPtr& swap_chain)
{
	auto dsv = reinterpret_cast<ID3D11DepthStencilView*>(swap_chain->getDepthStencilView());

	m_immContext->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void CXGraphicsEngine::clearColor(const  CXTexture2DPtr& render_target, const CXVec4& color)
{
	if (render_target->m_desc.type != CXTextureType::RenderTarget) return;

	FLOAT clear_color[] = { color.x, color.y, color.z, color.w };
	m_immContext->ClearRenderTargetView(render_target->m_render_target_view.Get(), clear_color);
}

void CXGraphicsEngine::clearDepthStencil(const  CXTexture2DPtr& depth_stencil)
{

	if (depth_stencil->m_desc.type != CXTextureType::DepthStencil) return;
	m_immContext->ClearDepthStencilView((ID3D11DepthStencilView*)depth_stencil->m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void CXGraphicsEngine::setRenderTarget(const  CXTexture2DPtr& render_target, const  CXTexture2DPtr& depth_stencil)
{
	if (render_target->m_desc.type != CXTextureType::RenderTarget) return;
	if (depth_stencil->m_desc.type != CXTextureType::DepthStencil) return;

	m_immContext->OMSetRenderTargets(1, render_target->m_render_target_view.GetAddressOf(), (ID3D11DepthStencilView*)depth_stencil->m_depth_stencil_view.Get());
}


void CXGraphicsEngine::setVertexBuffer(const  CXVertexBufferPtr& buffer)
{
	ui32 stride = buffer->getVertexSize();
	ui32 offset = 0;
	auto buf = reinterpret_cast<ID3D11Buffer*>(buffer->getBuffer());
	m_immContext->IASetVertexBuffers(0, 1, &buf, &stride, &offset);
	m_immContext->IASetInputLayout((ID3D11InputLayout*)buffer->getLayout());
}

void CXGraphicsEngine::setIndexBuffer(const  CXIndexBufferPtr& buffer)
{
	m_immContext->IASetIndexBuffer((ID3D11Buffer*)buffer->getBuffer(), DXGI_FORMAT_R32_UINT, 0);
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

void CXGraphicsEngine::setShader(const CXShaderPtr& shader)
{
	m_immContext->VSSetShader((ID3D11VertexShader*)shader->getVertexShader(), nullptr, 0);
	m_immContext->PSSetShader((ID3D11PixelShader*)shader->getPixelShader(), nullptr, 0);
}

void CXGraphicsEngine::setTexture(const  CXTexturePtr* texture, unsigned int num_textures)
{
	ID3D11ShaderResourceView* list_res[32];
	ID3D11SamplerState* list_sampler[32];
	for (unsigned int i = 0; i < num_textures; i++)
	{
		list_res[i] = (ID3D11ShaderResourceView*)texture[i]->m_texture->m_shader_res_view.Get();
		list_sampler[i] = (ID3D11SamplerState*)texture[i]->m_texture->m_sampler_state.Get();
	}

	m_immContext->VSSetShaderResources(0, num_textures, list_res);
	m_immContext->VSSetSamplers(0, num_textures, list_sampler);
	m_immContext->PSSetShaderResources(0, num_textures, list_res);
	m_immContext->PSSetSamplers(0, num_textures, list_sampler);
}

void CXGraphicsEngine::setConstantBuffer(const  CXConstantBufferPtr& buffer)
{
	auto buf = reinterpret_cast<ID3D11Buffer*>(buffer->getBuffer());
	m_immContext->VSSetConstantBuffers(0, 1, &buf);
	m_immContext->PSSetConstantBuffers(0, 1, &buf);
}

void CXGraphicsEngine::setMaterial(const  CXMaterialPtr& material)
{
	setRasterizerState((material->m_cull_mode == CXCullMode::Front));

	if (material->m_constant_buffer)
		setConstantBuffer(material->m_constant_buffer);

	setShader(material->m_shader);

	if (material->m_vec_textures.size())
	{

		setTexture(&material->m_vec_textures[0], (UINT)material->m_vec_textures.size());
	}
	
}

void CXGraphicsEngine::drawMesh(const  CXMeshPtr& mesh, const std::vector<CXMaterialPtr>& list_materials)
{
	for (unsigned int m = 0; m < list_materials.size(); m++)
	{
		if (m == list_materials.size()) break;

		CXMaterialSlot mat = mesh->getMaterialSlot(m);

		setMaterial(list_materials[m]);
		setVertexBuffer(mesh->getVertexBuffer());
		setIndexBuffer(mesh->getIndexBuffer());
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
