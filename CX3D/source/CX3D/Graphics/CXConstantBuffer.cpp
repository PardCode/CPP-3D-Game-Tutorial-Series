/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

Copyright (c) 2019-2024, PardCode

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

#include <CX3D/Graphics/CXConstantBuffer.h>
#include <CX3D/Graphics/CXGraphicsEngine.h>

#include <stdexcept>
#include <d3d11.h>

CXConstantBuffer::CXConstantBuffer(const CXConstantBufferDesc& desc, CXGraphicsEngine* system) : m_system(system)
{
	D3D11_BUFFER_DESC buff_desc = {};
	buff_desc.Usage = D3D11_USAGE_DEFAULT;
	buff_desc.ByteWidth = desc.bufferSize;
	buff_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buff_desc.CPUAccessFlags = 0;
	buff_desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA init_data = {};
	init_data.pSysMem = desc.buffer;
	auto device = m_system->m_d3dDevice;

	if (FAILED(device->CreateBuffer(&buff_desc, &init_data, &m_buffer)))
		throw std::runtime_error("CXConstantBuffer not created successfully");
}

void CXConstantBuffer::update(void* buffer)
{
	m_system->m_immContext->UpdateSubresource(this->m_buffer.Get(), NULL, NULL, buffer, NULL, NULL);
}

void* CXConstantBuffer::getBuffer()
{
	return m_buffer.Get();
}
