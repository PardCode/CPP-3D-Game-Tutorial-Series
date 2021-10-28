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
#include <vector>
#include <DX3D/Resource/Resource.h>
enum CULL_MODE

{
	CULL_MODE_FRONT=0,
	CULL_MODE_BACK
};

class RenderSystem;
class Material : public Resource
{
public:
	Material(const wchar_t* full_path, ResourceManager* manager);
	Material(const MaterialPtr & material, ResourceManager* manager);
	~Material();

	void addTexture(const TexturePtr& texture);
	void removeTexture(unsigned int index);

	void setData(void * data, unsigned int size);
	void setUserData(void* data, unsigned int size);

	void setCullMode(CULL_MODE mode);
	CULL_MODE getCullMode();
private:
	VertexShaderPtr m_vertex_shader;
	PixelShaderPtr m_pixel_shader;
	ConstantBufferPtr m_constant_buffer;
	ConstantBufferPtr m_userBuffer;
	std::vector<TexturePtr> m_vec_textures;
	CULL_MODE m_cull_mode = CULL_MODE_BACK;
	friend class RenderSystem;
};

