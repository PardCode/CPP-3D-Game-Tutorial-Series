#include "VertexShader.h"
#include "GraphicsEngine.h"


VertexShader::VertexShader()
{
}

void VertexShader::release()
{
	m_vs->Release();
	delete this;
}

bool VertexShader::init(const void* shader_byte_code, size_t byte_code_size)
{
	if (!SUCCEEDED(GraphicsEngine::get()->m_d3d_device->CreateVertexShader(shader_byte_code, byte_code_size, nullptr, &m_vs)))
		return false;

	return true;
}



VertexShader::~VertexShader()
{
}


