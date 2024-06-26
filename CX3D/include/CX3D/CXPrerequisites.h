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

#pragma once
#include <memory>
#include <sstream>
#include <iostream>
#include <stdexcept>

class  CXWindow;
class  CXDisplay;

class  CXGraphicsEngine;
class  CXShader;
class  CXSwapChain;
class  CXVertexBuffer;
class  CXIndexBuffer;
class  CXConstantBuffer;
class  CXTexture2D;

class CXInputManager;


class  CXResourceManager;
class  CXResource;
class  CXTexture;
class  CXMesh;
class  CXMaterial;



class  CXGame;
class  CXEntity;
class  CXMeshEntity;
class  CXLightEntity;
class  CXCameraEntity;




typedef std::shared_ptr<CXSwapChain> CXSwapChainPtr;
typedef std::shared_ptr<CXVertexBuffer> CXVertexBufferPtr;
typedef std::shared_ptr<CXIndexBuffer> CXIndexBufferPtr;
typedef std::shared_ptr<CXConstantBuffer> CXConstantBufferPtr;
typedef std::shared_ptr<CXResource> CXResourcePtr;
typedef std::shared_ptr<CXTexture> CXTexturePtr;
typedef std::shared_ptr<CXMesh> CXMeshPtr;
typedef std::shared_ptr<CXMaterial> CXMaterialPtr;
typedef std::shared_ptr<CXShader> CXShaderPtr;
typedef std::shared_ptr<CXTexture2D> CXTexture2DPtr;


typedef unsigned int ui32;
typedef int i32;
typedef float f32;
typedef double d64;


enum class  CXKey
{
	Escape = 0,
	Shift,
	Space,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z,
	N0,
	N1,
	N2,
	N3,
	N4,
	N5,
	N6,
	N7,
	N8,
	N9,
	F1,
	F2,
	F3,
	F4,
	F5,
	F6,
	F7,
	F8,
	F9,
	F10,
	F11,
	F12,
};

enum class  CXTextureType
{
	Normal = 0,
	RenderTarget,
	DepthStencil
};

enum class  CXMouseButton
{
	Left = 0,
	Middle,
	Right
};


enum class  CXCullMode
{
	Front = 0,
	Back
};


struct CXMaterialSlot
{
	size_t start_index = 0;
	size_t num_indices = 0;
	size_t material_id = 0;
};

struct CXConstantBufferDesc
{
	void* buffer = nullptr;
	ui32 bufferSize = 0;
};


struct CXVertexBufferDesc
{
	void* verticesList = nullptr;
	ui32 vertexSize = 0;
	ui32 listSize = 0;
};

struct CXIndexBufferDesc
{
	void* indicesList = nullptr;
	ui32 listSize = 0;
};

struct CXShaderDesc
{
	const wchar_t* vertexShaderFilePath;
	const char* vertexShaderEntryPointName;
	
	const wchar_t* pixelShaderFilePath;
	const char* pixelShaderEntryPointName;
};


#define CX3D_ERROR(message)\
{\
std::stringstream m;\
m << "CX3D Error: " << message << std::endl;\
throw std::runtime_error(m.str());\
}

#define CX3D_WARNING(message)\
std::wclog << "CX3D Warning: " << message << std::endl;


#define CX3D_INFO(message)\
std::wclog << "CX3D Info: " << message << std::endl;