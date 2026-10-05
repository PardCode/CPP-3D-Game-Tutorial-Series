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
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Core.h>
#include <chrono>

namespace dx3d
{
	class Game
	{
		dx3d_disable_copy_and_move(Game)
	public:
		explicit Game(const GameDesc& desc);
		virtual ~Game();

		virtual World& getWorld() noexcept final;
		virtual Logger& getLogger() const noexcept final;
		virtual InputSystem& getInputSystem() noexcept final;
		virtual ResourceManager& getResourceManager() noexcept final;

		virtual void run() final;
	protected:
		virtual void onCreate() {}
		virtual void onUpdate(f32 deltaTime) {}
	private:
		void onInternalUpdate();
	private:
		UniquePtr<Logger> m_logger{};
		UniquePtr<InputSystem> m_inputSystem{};
		RefPtr<GraphicsDevice> m_graphicsDevice{};
		UniquePtr<Display> m_display{};
		UniquePtr<ResourceManager> m_resourceManager{};
		UniquePtr<World> m_world{};	
		UniquePtr<WorldPhysicsSystem> m_worldPhysicsSystem{};	
		UniquePtr<WorldRenderer> m_worldRenderer{};

		bool m_isRunning{ true };

		std::chrono::steady_clock::time_point m_previousTime{};
	};
}
