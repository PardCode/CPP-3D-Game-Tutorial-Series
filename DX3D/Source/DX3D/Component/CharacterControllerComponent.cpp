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

#include <DX3D/Component/CharacterControllerComponent.h>

dx3d::CharacterControllerComponent::CharacterControllerComponent(const ComponentDesc& data): Component(data)
{
}

void dx3d::CharacterControllerComponent::setHeight(f32 height) noexcept
{
	m_height = height;
}

dx3d::f32 dx3d::CharacterControllerComponent::getHeight() const noexcept
{
	return m_height;
}

dx3d::Vec3 dx3d::CharacterControllerComponent::getMoveDirection() const noexcept
{
	return m_direction;
}

dx3d::f32 dx3d::CharacterControllerComponent::getMoveDistance() const noexcept
{
	return m_distance;
}

void dx3d::CharacterControllerComponent::move(const Vec3& direction, f32 distance)
{
	m_distance = distance;
	m_direction = Vec3::normalize(direction);
}
