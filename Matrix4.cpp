#include "Matrix4.h"
#include <cmath>
//�P�ʍs������߂�
Matrix4 identity()
{
	static const Matrix4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}
//�g��k���s��̐ݒ�
Matrix4 Scale(const Vector3& s)
{
	static const Matrix4 result{
		 s.x,0.0f,0.0f,0.0f,
		0.0f, s.y,0.0f,0.0f,
		0.0f,0.0f, s.z,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
	return result;
}
//��]�s��̐ݒ�
Matrix4 RotateX(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	 Matrix4 result{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,  cos,  sin, 0.0f,
		0.0f, -sin,  cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}
Matrix4 RotateY(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	 Matrix4 result{
		 cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		 sin, 0.0f,  cos, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}
Matrix4 RotateZ(float angle)
{
	float sin = std::sin(angle);
	float cos = std::cos(angle);

	 Matrix4 result{
		 cos,  sin, 0.0f, 0.0f,
		-sin,  cos, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};

	return result;
}
//���s�ړ��s��̍쐬
Matrix4 translate(const Vector3& t)
{
	static const Matrix4 result{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		 t.x, t.y, t.z,1.0f,
	};
	return result;
}
//���W�ϊ��i�x�N�g���ƍs��̊|���Z������j
Vector3 transform(const Vector3& v, const Matrix4& m)
{
	float w = (v.x * m.m[0][3]) + (v.y * m.m[1][3]) + (v.z * m.m[2][3]) + m.m[3][3];

	Vector3 result
	{
		((v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]) + m.m[3][0]) / w,
		((v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]) + m.m[3][1]) / w,
		((v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]) + m.m[3][2]) / w
	};

	return result;
}
//������Z�q�I�[�o�[���[�h
Matrix4& operator*=(Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}
//2�����Z�q�I�[�o�[���[�h
Matrix4 operator*(const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;
	return result *= m2;
}
Vector3 operator*(const Vector3& v, const Matrix4& m)
{
	return transform(v,m);
}


