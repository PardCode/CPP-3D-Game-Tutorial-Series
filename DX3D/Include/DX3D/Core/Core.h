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
#include <stdexcept>
#include <memory>

#define dx3d_disable_copy_and_move(Class)     \
protected:\
    Class(const Class&) = delete;        \
    Class& operator=(const Class&) = delete; \
    Class(Class&&) = delete;             \
    Class& operator=(Class&&) = delete;

namespace dx3d
{
	class Base;
	class Window;
	class Game;
	class InputSystem;
	class GraphicsEngine;
	class GraphicsDevice;
	class Logger;
	class SwapChain;
	class Display;
	class DeviceContext;
	class ShaderBinary;
	class GraphicsPipelineState;
	class VertexBuffer;
	class VertexShaderSignature;
	class ConstantBuffer;
	class IndexBuffer;
	class Texture;
	class Sampler;
	class GraphicsPipelineLayout;

	class World;
	class GameObject;
	class Component;
	class TransformComponent;

	class WorldRenderer;
	class WorldPhysicsSystem;


	class ResourceManager;
	class Resource;
	class MaterialResource;
	class TextureResource;
	class MeshResource;




	using i32 = int;
	using ui32 = unsigned int;
	using f32 = float;
	using d64 = double;

	template <typename T> using RefPtr = std::shared_ptr<T>;
	template <typename T> using UniquePtr = std::unique_ptr<T>;
}