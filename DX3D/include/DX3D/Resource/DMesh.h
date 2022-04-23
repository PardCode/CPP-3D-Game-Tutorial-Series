/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2022, PardCode

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
#include <DX3D/Resource/DResource.h>
#include <DX3D/DPrerequisites.h>
#include <DX3D/Math/DVec3.h>
#include <DX3D/Math/DVec2.h>
#include <DX3D/Math/DVertexMesh.h>
#include <vector>


class DMesh: public DResource
{
public:
	DMesh(const wchar_t* full_path, DResourceManager* manager);
	DMesh(DVertexMesh * vertex_list_data, unsigned int vertex_list_size,
		unsigned int * index_list_data, unsigned int index_list_size,
		DMaterialSlot * material_slot_list, unsigned int material_slot_list_size, DResourceManager* manager);
	const DVertexBufferPtr& getVertexBuffer();
	const DIndexBufferPtr& getIndexBuffer();

	const DMaterialSlot& getMaterialSlot(unsigned int slot);
	size_t getNumMaterialSlots();

	DVec3 getMaxCorner();
	DVec3 getMinCorner();
private:
	void computeTangents(
		const DVec3& v0, const DVec3& v1, const DVec3& v2,
		const DVec2& t0, const DVec2& t1, const DVec2& t2,
		DVec3& tangent, DVec3& binormal);
private:
	DVertexBufferPtr m_vertex_buffer;
	DIndexBufferPtr m_index_buffer;
	std::vector<DMaterialSlot> m_mat_slots;

	DVec3 m_maxCorner;
	DVec3 m_minCorner = DVec3(10000,10000,10000);

private:
	friend class DGraphicsEngine;
};

