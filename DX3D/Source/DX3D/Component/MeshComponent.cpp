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

#include <DX3D/Component/MeshComponent.h>
#include <DX3D/Game/GameObject.h>
#include <DX3D/Game/World.h>
#include <DX3D/Game/Game.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Resource/MaterialResource.h>
#include <DX3D/Resource/MeshResource.h>


dx3d::MeshComponent::MeshComponent(const ComponentDesc& data) : Component(data)
{
}

void dx3d::MeshComponent::setMesh(const RefPtr<MeshResource>& mesh)
{
	m_mesh = mesh;
	if (m_mesh) m_materials.resize(m_mesh->getNumMaterialSlots());
	else m_materials.resize(0);
}

const dx3d::MeshResource* dx3d::MeshComponent::getMesh() const noexcept
{
	return m_mesh.get();
}

void dx3d::MeshComponent::setMaterial(dx3d::ui32 index, const RefPtr<MaterialResource>& material)
{
	if (index >= m_materials.size())
	{
		DX3DLogError("Index {} is out of bounds for the materials list (size: {}). Ensure setMesh() has been called before setting materials.", index, m_materials.size());
		return;
	}

	m_materials[index] = material;
}

const dx3d::MaterialResource* dx3d::MeshComponent::getMaterial(ui32 index) const noexcept
{
	if (index >= m_materials.size())
	{
		DX3DLogError("Index {} is out of bounds for the materials list (size: {}).", index, m_materials.size());
		return {};
	}
	return m_materials[index].get();
}
