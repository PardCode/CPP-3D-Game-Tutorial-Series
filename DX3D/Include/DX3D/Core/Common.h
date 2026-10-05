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
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec2.h>

namespace dx3d
{
	struct BaseDesc
	{
		Logger& logger;
	};

	struct WindowDesc
	{
		BaseDesc base;
		Rect size{};
	};

	struct DisplayDesc
	{
		WindowDesc window;
		GraphicsDevice& graphicsDevice;
	};

	struct GraphicsDeviceDesc
	{
		BaseDesc base;
	};

	struct SwapChainDesc
	{
		void* winHandle{};
		Rect winSize{};
	};

	enum class ShaderType
	{
		VertexShader = 0,
		PixelShader
	};

	struct ShaderCompileDesc
	{
		const char* shaderSourceName{};
		const void* shaderSourceCode{};
		size_t shaderSourceCodeSize{};
		const char* shaderEntryPoint{};
		ShaderType shaderType{};
	};

	struct GraphicsPipelineLayoutDesc
	{
		const RefPtr<ShaderBinary>& vsBinary;
		const RefPtr<ShaderBinary>& psBinary;
	};

	struct BinaryData
	{
		const void* data{};
		size_t dataSize{};
	};

	struct GraphicsPipelineStateDesc
	{
		const GraphicsPipelineLayout& layout;
	};

	struct VertexBufferDesc
	{
		const void* vertexList{};
		ui32 vertexListSize{};
		ui32 vertexSize{};
	};

	struct ConstantBufferDesc
	{
		const void* buffer{};
		ui32 bufferSize{};
	};

	struct IndexBufferDesc
	{
		const ui32* indexList{};
		ui32 indexListSize{};
	};



	struct GameContext
	{
		InputSystem& input;
		ResourceManager& resourceManager;
		GraphicsDevice& device;
	};

	struct GameDesc
	{
		Rect windowSize{ 1280,720 };
		Logger::LogLevel logLevel = Logger::LogLevel::Error;
	};

	struct WorldDesc 
	{
		BaseDesc base;
		GameContext gameContext;
	};

	struct GameObjectDesc
	{
		BaseDesc base;
		GameContext gameContext;
		World& world;
	};

	struct ComponentDesc
	{
		BaseDesc base;
		GameObject& object;
		World& world;
		GameContext& context;
	};

	struct WorldRendererDesc
	{
		BaseDesc base;
		GraphicsDevice& engine;
	};

	struct WorldPhysicsSystemDesc
	{
		BaseDesc base;
	};

	enum class KeyCode
	{
		Unknown = 0,
		// Letters
		A, B, C, D, E, F, G,
		H, I, J, K, L, M, N,
		O, P, Q, R, S, T, U,
		V, W, X, Y, Z,

		// Numbers
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,


		Escape,
		Shift,
		Space,
		Enter,

		// Mouse buttons (optional inclusion)
		MouseLeft,
		MouseRight,
		MouseMiddle,

		// Arrows
		Up,
		Down,
		Left,
		Right,

		Count
	};

	struct InputSystemDesc
	{
		BaseDesc base;
	};

	struct ResourceDesc
	{
		BaseDesc base;
		const wchar_t* path{};
		ResourceManager& manager;
	};

	struct MeshResourceDesc
	{
		ResourceDesc base;
		GraphicsDevice& graphicsDevice;
	};
	struct MaterialResourceDesc
	{
		ResourceDesc base;
		GraphicsDevice& graphicsDevice;
	};
	struct TextureResourceDesc
	{
		ResourceDesc base;
		GraphicsDevice& graphicsDevice;
	};

	struct SystemContext
	{
		GraphicsDevice& graphicsDevice;
	};
		
	struct ResourceManagerDesc
	{
		BaseDesc base;
		SystemContext context;
	};

	struct TextureDesc
	{
		Rect size{};
		const void* pixels{};
	};
	struct SamplerDesc
	{
	};


	struct MeshVertex
	{
		Vec3 position{};
		Vec2 texcoord{};
		Vec3 normal{};
	};

	struct MaterialSlot
	{
		ui32 startIndex{};
		ui32 indexCount{};
		i32 materialIndex{};
	};






}