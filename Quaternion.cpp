#include "Quaternion.h"

//�P��Quaternion��Ԃ�
Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion identity = { 0,0,0,1 };
	

	return identity;
}
//����Quaternion��Ԃ�
Quaternion Quaternion::Conjugate() const
{
	Quaternion result = {
	this->x * -1,
	this->y * -1,
	this->z * -1,
	w
	};
	return result;
}
//Quaternion��norm��Ԃ�
float Quaternion::Norm()
{
	return sqrt(w * w + x * x + y * y + z * z);
}
//���K������Quaternion��Ԃ�
Quaternion Quaternion::Normalize()
{
	Quaternion result = {
		this->x / Norm(),
		this->y / Norm(),
		this->z / Norm(),
		this->w / Norm(),
	};

	return result;
}
//�tQuaternion��Ԃ�
Quaternion Quaternion::Inverse()
{
	Quaternion result = {
		this->x / pow(Norm(),2),
		this->y / pow(Norm(),2),
		this->z / pow(Norm(),2),
		this->w / pow(Norm(),2),
	};

	return result;
}
//Quaternion�̐�
Quaternion Quaternion::Multiply(const Quaternion& rhs)const 
{
	Quaternion result = {
		this->x * rhs.w + this->w * rhs.x - this->z * rhs.y + this->y * rhs.z,
		this->y * rhs.w + this->z * rhs.x + this->w * rhs.y - this->x * rhs.z,
		this->z * rhs.w - this->y * rhs.x + this->x * rhs.y + this->w * rhs.z,
		this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z
	};

	return result;
}
//�C�ӎ���]��\��Quaternion�̐���
Quaternion MakeAxisAngle(const Vector3& axis, float angle)
{
	float Sin = sin(angle / 2.f);

	Vector3 vector = axis;
	vector = vector.normalize();
	Quaternion result = {
		vector.x * Sin,
		vector.y * Sin,
		vector.z * Sin,
		cos(angle / 2.f)
	};

	return result;
}
//�x�N�g����Quaternion�ŉ�]���������ʂ̃x�N�g�������߂�
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{

	Quaternion conj = quaternion.Conjugate();

	Quaternion q = {
		vector.x,
		vector.y,
		vector.z,
		0
	};

	Quaternion result = quaternion.Multiply(q);

	result = result.Multiply(conj);

	//�N�H�[�^�j�I���~�x�N�g���~�����N�H�[�^�j�I��
	return Vector3(result.x, result.y, result.z);
}


