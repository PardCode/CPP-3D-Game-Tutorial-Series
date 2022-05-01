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

#pragma once
#include <memory>
#include <CX3D/Math/CXVec3.h>
#include <CX3D/Math/CXVec4.h>
#include <CX3D/CXPrerequisites.h>


class  CXMat4
{
public:
	CXMat4()
	{
		setIdentity();
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(m_mat));
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const  CXVec3& translation)
	{
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}

	void setScale(const  CXVec3& scale)
	{
		m_mat[0][0] = (f32)scale.x;
		m_mat[1][1] = (f32)scale.y;
		m_mat[2][2] = (f32)scale.z;
	}

	void setRotationX(f32 x)
	{
		m_mat[1][1] = (f32)cos(x);
		m_mat[1][2] = (f32)sin(x);
		m_mat[2][1] = (f32)-sin(x);
		m_mat[2][2] = (f32)cos(x);
	}

	void setRotationY(f32 y)
	{
		m_mat[0][0] = (f32)cos(y);
		m_mat[0][2] = (f32)-sin(y);
		m_mat[2][0] = (f32)sin(y);
		m_mat[2][2] = (f32)cos(y);
	}

	void setRotationZ(f32 z)
	{
		m_mat[0][0] = (f32)cos(z);
		m_mat[0][1] = (f32)sin(z);
		m_mat[1][0] = (f32)-sin(z);
		m_mat[1][1] = (f32)cos(z);
	}

	f32 getDeterminant()
	{
		CXVec4 minor, v1, v2, v3;
		f32 det = 0.0f;

		v1 = CXVec4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = CXVec4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = CXVec4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
			this->m_mat[3][3] * minor.w);
		return det;
	}

	void inverse()
	{
		i32 a = 0, i = 0, j = 0;
		CXMat4 out = {};
		CXVec4 v = {}, vec[3] = {};
		f32 det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->m_mat[j][0]);
					vec[a].y = (this->m_mat[j][1]);
					vec[a].z = (this->m_mat[j][2]);
					vec[a].w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = (f32)pow(-1.0f, i) * v.x / det;
			out.m_mat[1][i] = (f32)pow(-1.0f, i) * v.y / det;
			out.m_mat[2][i] = (f32)pow(-1.0f, i) * v.z / det;
			out.m_mat[3][i] = (f32)pow(-1.0f, i) * v.w / det;
		}

		this->setMatrix(out);
	}


	void operator *=(const  CXMat4& m_matrix)
	{
		CXMat4 out;
		for (i32 i = 0; i < 4; i++)
		{
			for (i32 j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * m_matrix.m_mat[0][j] + m_mat[i][1] * m_matrix.m_mat[1][j] +
					m_mat[i][2] * m_matrix.m_mat[2][j] + m_mat[i][3] * m_matrix.m_mat[3][j];
			}
		}
		setMatrix(out);
	}

	void setMatrix(const  CXMat4& m_matrix)
	{
		::memcpy(m_mat, m_matrix.m_mat, sizeof(m_mat));
	}

	CXVec3 getRightwardDirection()
	{
		return CXVec3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	CXVec3 getUpDirection()
	{
		return CXVec3(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}
	CXVec3 getForwardDirection()
	{
		return CXVec3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	CXVec3 getTranslation()
	{
		return CXVec3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(f32 fov, f32 aspect, f32 znear, f32 zfar)
	{
		f32 yscale = 1.0f / (f32)tan(fov / 2.0f);
		f32 xscale = yscale / aspect;
		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
		m_mat[3][3] = 0.0f;

	}


	void setOrthoLH(f32 width, f32 height, f32 near_plane, f32 far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

public:
	f32 m_mat[4][4] = {};
};