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

#pragma once
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Math/Vec4.h>
#include <span>
#include <array>

namespace dx3d
{
	class DeviceContext final: public GraphicsResource
	{
	public:
		explicit DeviceContext(const GraphicsResourceDesc& gDesc);
		void clearAndSetBackBuffer(const SwapChain& swapChain, const Vec4& color);
		void setGraphicsPipelineState(const GraphicsPipelineState& pipeline);
		void setVertexBuffer(const VertexBuffer& buffer);
		void setIndexBuffer(const IndexBuffer& buffer);
		void setViewportSize(const Rect& size);
		void setConstantBuffers(const std::span<const ConstantBuffer*>& buffers);
		void setTextures(const std::span<const Texture*>& textures);
		void setSamplers(const std::span<const Sampler*>& samplers);
		void updateConstantBuffer(const ConstantBuffer& buffer, const std::span<const std::byte>& data);
		void drawTriangleList(ui32 vertexCount, ui32 startVertexLocation);
		void drawIndexedTriangleList(ui32 indexCount, ui32 startVertexIndex, ui32 startIndexLocation);
	public:
		static constexpr std::size_t MaxConstantBuffersPerStage{ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT };
		static constexpr std::size_t MaxSamplersPerStage{ D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT };
		static constexpr std::size_t MaxTexturesPerStage{ D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT };
	private:
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context{};
		std::array<ID3D11Buffer*,MaxConstantBuffersPerStage> m_constantBuffers{};
		std::array<ID3D11ShaderResourceView*,MaxTexturesPerStage> m_srv{};
		std::array<ID3D11SamplerState*,MaxSamplersPerStage> m_samplers{};

		friend class GraphicsDevice;
	};
}

