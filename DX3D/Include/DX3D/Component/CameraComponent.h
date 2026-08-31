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
#include <DX3D/Game/Component.h>
#include <DX3D/Math/Rect.h>
#include <DX3D/Math/Mat4x4.h>

namespace dx3d
{
	class CameraComponent final : public Component
	{
		dx3d_typeid(CameraComponent)
	public:
		explicit CameraComponent(const ComponentDesc& data);

		Mat4x4 getViewMatrix() noexcept;
		Mat4x4 getProjectionMatrix() const noexcept;

		void setFarPlane(f32 farPlane) noexcept;
		f32 getFarPlane() const noexcept;

		void setNearPlane(f32 nearPlane) noexcept;
		f32 getNearPlane()const noexcept;

		void setFieldOfView(f32 fieldOfView) noexcept;
		f32 getFieldOfView() const noexcept;

		void setViewportSize(const Rect& size) noexcept;
		Rect getViewportSize() const noexcept;

	private:
		void computeProjectionMatrix() noexcept;

	private:		
		Mat4x4 m_projection{};

		f32 m_nearPlane{ 0.01f };
		f32 m_farPlane{ 1500.0f };
		f32 m_fieldOfView{ 1.3f };
		Rect m_viewportSize { 1,1 };

		bool m_dirty{ true };
	};

}
