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

#include <DX3D/Graphics/VertexBuffer.h>

dx3d::VertexBuffer::VertexBuffer(const VertexBufferDesc& desc, const GraphicsResourceDesc& gDesc): 
	GraphicsResource(gDesc), m_vertexSize(desc.vertexSize), m_vertexListSize(desc.vertexListSize)
{
	if (!desc.vertexList) DX3DLogThrowInvalidArg("No vertex list provided.");
	if (!desc.vertexListSize) DX3DLogThrowInvalidArg("Vertex list size must be non-zero.");
	if (!desc.vertexSize) DX3DLogThrowInvalidArg("Vertex size must be non-zero.");

	D3D11_BUFFER_DESC buffDesc{};
	buffDesc.ByteWidth = desc.vertexListSize * desc.vertexSize;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = desc.vertexList;

	DX3DGraphicsLogThrowOnFail(
		m_device.CreateBuffer(&buffDesc, &initData, &m_buffer),
		"CreateBuffer failed.");
}

dx3d::ui32 dx3d::VertexBuffer::getVertexListSize() const noexcept
{
	return m_vertexListSize;
}
