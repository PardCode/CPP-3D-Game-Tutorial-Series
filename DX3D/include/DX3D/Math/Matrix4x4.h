/*MIT License

C++ 3D Game Tutorial Series (https://github.com/PardCode/CPP-3D-Game-Tutorial-Series)

Copyright (c) 2019-2021, PardCode

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
#include <DX3D/Math/Vector3D.h>
#include <DX3D/Math/Vector4D.h>
#include <DX3D/Prerequisites.h>


class Matrix4x4
{
public:
	Matrix4x4()
	{
		setIdentity();
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTranslation(const Vector3D& translation)
	{
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}

	void setScale(const Vector3D& scale)
	{
		m_mat[0][0] = (f32)scale.x;
		m_mat[1][1] = (f32)scale.y;
		m_mat[2][2] = (f32)scale.z;
	}

	void setRotationX(float x)
	{
		m_mat[1][1] = (f32)cos(x);
		m_mat[1][2] = (f32)sin(x);
		m_mat[2][1] = (f32)-sin(x);
		m_mat[2][2] = (f32)cos(x);
	}

	void setRotationY(float y)
	{
		m_mat[0][0] = (f32)cos(y);
		m_mat[0][2] = (f32)-sin(y);
		m_mat[2][0] = (f32)sin(y);
		m_mat[2][2] = (f32)cos(y);
	}

	void setRotationZ(float z)
	{
		m_mat[0][0] = (f32)cos(z);
		m_mat[0][1] = (f32)sin(z);
		m_mat[1][0] = (f32)-sin(z);
		m_mat[1][1] = (f32)cos(z);
	}

	float getDeterminant()
	{
		Vector4D minor, v1, v2, v3;
		float det;

		v1 = Vector4D(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4D(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4D(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
			this->m_mat[3][3] * minor.w);
		return det;
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4D v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i<4; i++)
		{
			for (j = 0; j<4; j++)
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


	void operator *=(const Matrix4x4& m_matrix)
	{
		Matrix4x4 out;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * m_matrix.m_mat[0][j] + m_mat[i][1] * m_matrix.m_mat[1][j] + 
					m_mat[i][2] * m_matrix.m_mat[2][j] + m_mat[i][3] * m_matrix.m_mat[3][j];
			}
		}
		setMatrix(out);
	}

	void setMatrix(const Matrix4x4& m_matrix)
	{
		::memcpy(m_mat, m_matrix.m_mat, sizeof(float) * 16);
	}

	Vector3D getXDirection()
	{
		return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}
	Vector3D getYDirection()
	{
		return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}
	Vector3D getZDirection()
	{
		return Vector3D(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}
	
	Vector3D getTranslation()
	{
		return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	void setPerspectiveFovLH(float fov, float aspect, float znear, float zfar)
	{
		float yscale = 1.0f / (f32)tan(fov / 2.0f);
		float xscale = yscale / aspect;
		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear*zfar)/ (zfar - znear);
	}


	void setOrthoLH(float width,float height,float near_plane, float far_plane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(near_plane / (far_plane - near_plane));
	}

	~Matrix4x4()
	{
	}

public:
	float m_mat[4][4] = {};
};