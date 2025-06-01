/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2025, PardCode

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

#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Math/Vec3.h>

using namespace dx3d;

dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc): Base(desc.base)
{
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{m_logger});

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	constexpr char shaderSourceCode[] =
		R"(
float4 VSMain(float3 pos: POSITION): SV_Position
{
return float4(pos.xyz, 1.0);
}
float4 PSMain(): SV_Target
{
return float4(1.0, 1.0, 1.0, 1.0);
}
)";
	constexpr char shaderSourceName[] = "Basic";
	constexpr auto shaderSourceCodeSize = std::size(shaderSourceCode);

	auto vs = device.compileShader({shaderSourceName, shaderSourceCode, shaderSourceCodeSize,
		"VSMain", ShaderType::VertexShader});
	auto ps = device.compileShader({ shaderSourceName, shaderSourceCode, shaderSourceCodeSize,
		"PSMain", ShaderType::PixelShader });

	m_pipeline = device.createGraphicsPipelineState({ *vs,*ps });

	const Vec3 vertexList[] =
	{
		{-0.5f, -0.5f, 0.0f},
		{0.0f, 0.5f, 0.0f},
		{0.5f, -0.5f, 0.0f}
	};

	m_vb = device.createVertexBuffer({vertexList, std::size(vertexList), sizeof(Vec3)});
}

dx3d::GraphicsEngine::~GraphicsEngine()
{
}

GraphicsDevice& dx3d::GraphicsEngine::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void dx3d::GraphicsEngine::render(SwapChain& swapChain)
{
	auto& context = *m_deviceContext;
	context.clearAndSetBackBuffer(swapChain, { 1,0,0,1 });
	context.setGraphicsPipelineState(*m_pipeline);

	context.setViewportSize(swapChain.getSize());

	auto& vb = *m_vb;
	context.setVertexBuffer(vb);
	context.drawTriangleList(vb.getVertexListSize(), 0u);

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();
}
