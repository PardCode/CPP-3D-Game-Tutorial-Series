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
#include <DX3D/Core/Base.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Vec2.h>
#include <DX3D/Math/Mat4x4.h>
#include <vector>

namespace dx3d
{
	class WorldRenderer final: public Base
	{
	public:
		explicit WorldRenderer(const WorldRendererDesc& desc);
		void render(const World& world, SwapChain& swapChain, f32 deltaTime);
	private:
		struct alignas(16) ObjectData
		{
			Mat4x4 affineWorld{};
			Mat4x4 rigidWorld{};
		};
		struct alignas(16) CameraData
		{
			Mat4x4 view{};
			Mat4x4 proj{};
			Vec3 position{};
		};
		struct alignas(16) DirectionalLightData
		{
			Vec3 color{}; f32 pad{};
			Vec3 direction{};
			f32 intensity{};
		};
		struct alignas(16) EnvironmentData
		{
			DirectionalLightData directionalLightData{};
			f32 time{};
		};
		struct alignas(16) TerrainData
		{
			Vec4 size{};
			float heightMapSize{};
		};
		struct alignas(16) WaterData
		{
			Vec4 size{};
			float wavesDisplacementTexSize{};
			f32 wavesSpeed{ 1.0f };
		};


	private:
		GraphicsDevice& m_graphicsDevice;
		RefPtr<DeviceContext> m_deviceContext{};
		RefPtr<ConstantBuffer> m_cameraCb{};
		RefPtr<ConstantBuffer> m_objectCb{};
		RefPtr<ConstantBuffer> m_envCb{};
		RefPtr<ConstantBuffer> m_materialCb{};
		RefPtr<ConstantBuffer> m_terrainCb{};
		RefPtr<ConstantBuffer> m_waterCb{};
		RefPtr<Sampler> m_sampler{};

		std::vector<const Texture*> m_textures{};
		f32 m_time{};
	};
}

