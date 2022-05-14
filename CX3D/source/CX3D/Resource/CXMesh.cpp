/*MIT License

CX3D Game Framework (https://github.com/PardCode/CX3D)

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
//#include <CX3D/Resource/CXGraphicsManager.h>
#include <CX3D/Resource/CXMesh.h>
#include <CX3D/Resource/CXResourceManager.h>
#include <CX3D/Game/CXGame.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include <locale>
#include <codecvt>
#include <filesystem>

#include <CX3D/Math/CXVertexMesh.h>

CXMesh::CXMesh(const wchar_t* full_path, CXResourceManager* manager) : CXResource(full_path, manager)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	auto inputfile = std::filesystem::path(full_path).string();//std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(full_path);

	std::string mtldir = inputfile.substr(0, inputfile.find_last_of("\\/"));


	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputfile.c_str(), mtldir.c_str());

	if (!err.empty()) throw std::runtime_error("DMesh not created successfully");

	if (!res) throw std::runtime_error("DMesh not created successfully");


	std::vector<CXVertexMesh> list_vertices;
	std::vector<unsigned int> list_indices;

	size_t vertex_buffer_size = 0;


	for (size_t s = 0; s < shapes.size(); s++)
	{
		vertex_buffer_size += shapes[s].mesh.indices.size();
	}


	list_vertices.reserve(vertex_buffer_size);
	list_indices.reserve(vertex_buffer_size);


	m_mat_slots.resize(materials.size());

	size_t index_global_offset = 0;

	for (size_t m = 0; m < materials.size(); m++)
	{
		m_mat_slots[m].start_index = index_global_offset;
		m_mat_slots[m].material_id = m;

		for (size_t s = 0; s < shapes.size(); s++)
		{
			size_t index_offset = 0;

			for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
			{
				if (shapes[s].mesh.material_ids[f] != m) {
					unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];
					index_offset += num_face_verts;
					continue;
				}

				unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

				CXVec3 vertices_face[3];
				CXVec2 texcoords_face[3];

				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[(int)index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[(int)index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[(int)index.vertex_index * 3 + 2];

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[(int)index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[(int)index.texcoord_index * 2 + 1];
					}
					vertices_face[v] = CXVec3(vx, vy, vz);
					texcoords_face[v] = CXVec2(tx, ty);
				}

				CXVec3 tangent, binormal;

				computeTangents(
					vertices_face[0], vertices_face[1], vertices_face[2],
					texcoords_face[0], texcoords_face[1], texcoords_face[2],
					tangent, binormal);


				for (unsigned char v = 0; v < num_face_verts; v++)
				{
					tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

					tinyobj::real_t vx = attribs.vertices[(int)index.vertex_index * 3 + 0];
					tinyobj::real_t vy = attribs.vertices[(int)index.vertex_index * 3 + 1];
					tinyobj::real_t vz = -attribs.vertices[(int)index.vertex_index * 3 + 2];



					m_maxCorner.x = (m_maxCorner.x < vx) ? vx : m_maxCorner.x;
					m_maxCorner.y = (m_maxCorner.y < vy) ? vy : m_maxCorner.y;
					m_maxCorner.z = (m_maxCorner.x < vz) ? vz : m_maxCorner.z;

					m_minCorner.x = (m_minCorner.x > vx) ? vx : m_minCorner.x;
					m_minCorner.y = (m_minCorner.y > vy) ? vy : m_minCorner.y;
					m_minCorner.z = (m_minCorner.x > vz) ? vz : m_minCorner.z;

					tinyobj::real_t tx = 0;
					tinyobj::real_t ty = 0;
					if (attribs.texcoords.size())
					{
						tx = attribs.texcoords[(int)index.texcoord_index * 2 + 0];
						ty = 1.0f - attribs.texcoords[(int)index.texcoord_index * 2 + 1];
					}

					tinyobj::real_t nx = 0;
					tinyobj::real_t ny = 0;
					tinyobj::real_t nz = 0;

					if (attribs.normals.size())
					{
						nx = attribs.normals[(int)index.normal_index * 3 + 0];
						ny = attribs.normals[(int)index.normal_index * 3 + 1];
						nz = -attribs.normals[(int)index.normal_index * 3 + 2];
					}

					CXVec3 v_tangent, v_binormal;
					v_binormal = CXVec3::cross(CXVec3(nx, ny, nz), tangent);
					v_tangent = CXVec3::cross(v_binormal, CXVec3(nx, ny, nz));


					CXVertexMesh vertex(CXVec3(vx, vy, vz), CXVec2(tx, ty), CXVec3(nx, ny, nz), v_tangent, v_binormal);
					list_vertices.push_back(vertex);

					list_indices.push_back((unsigned int)index_global_offset + v);
				}

				index_offset += num_face_verts;
				index_global_offset += num_face_verts;
			}
		}

		m_mat_slots[m].num_indices = index_global_offset - m_mat_slots[m].start_index;
	}
	auto engine = m_resManager->getGame()->getGraphicsEngine();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	m_vertex_buffer = engine->createVertexBuffer({ &list_vertices[0], sizeof(CXVertexMesh),
		(UINT)list_vertices.size() });
	m_index_buffer = engine->createIndexBuffer({ &list_indices[0], (UINT)list_indices.size() });
}

CXMesh::CXMesh(CXVertexMesh* vertex_list_data, unsigned int vertex_list_size,
	unsigned int* index_list_data, unsigned int index_list_size,
	CXMaterialSlot* material_slot_list, unsigned int material_slot_list_size, CXResourceManager* manager) :
	CXResource(L"", manager)
{
	auto engine = m_resManager->getGame()->getGraphicsEngine();

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	m_vertex_buffer = engine->createVertexBuffer({ vertex_list_data,
		sizeof(CXVertexMesh),
		(UINT)vertex_list_size });
	m_index_buffer = engine->createIndexBuffer({ index_list_data,
		(UINT)index_list_size });

	m_mat_slots.resize(material_slot_list_size);

	for (unsigned int i = 0; i < material_slot_list_size; i++)
	{
		m_mat_slots[i] = material_slot_list[i];
	}

}

const  CXVertexBufferPtr& CXMesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

const  CXIndexBufferPtr& CXMesh::getIndexBuffer()
{
	return m_index_buffer;
}

const  CXMaterialSlot& CXMesh::getMaterialSlot(unsigned int slot)
{
	return m_mat_slots[slot];
}

size_t CXMesh::getNumMaterialSlots()
{
	return m_mat_slots.size();
}

CXVec3 CXMesh::getMaxCorner()
{
	return m_maxCorner;
}

CXVec3 CXMesh::getMinCorner()
{
	return m_minCorner;
}

void CXMesh::computeTangents(const  CXVec3& v0,
	const  CXVec3& v1,
	const  CXVec3& v2,
	const  CXVec2& t0,
	const  CXVec2& t1,
	const  CXVec2& t2,
	CXVec3& tangent, CXVec3& binormal)
{
	CXVec3 deltaPos1 = v1 - v0;
	CXVec3 deltaPos2 = v2 - v0;

	CXVec2 deltaUV1 = t1 - t0;
	CXVec2 deltaUV2 = t2 - t0;


	float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
	tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y);
	tangent = CXVec3::normalize(tangent);
	binormal = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x);
	binormal = CXVec3::normalize(binormal);
}

