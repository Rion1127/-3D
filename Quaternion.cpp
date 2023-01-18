#include "Quaternion.h"

//単位Quaternionを返す
Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion identity = { 0,0,0,1 };
	

	return identity;
}
//共役Quaternionを返す
Quaternion Quaternion::Conjugate()
{
	Quaternion result = {
	this->x * -1,
	this->y * -1,
	this->z * -1,
	w
	};
	return result;
}
//Quaternionのnormを返す
float Quaternion::Norm()
{
	return sqrt(w * w + x * x + y * y + z * z);
}
//正規化したQuaternionを返す
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
//逆Quaternionを返す
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
//Quaternionの積
Quaternion Quaternion::Multiply(const Quaternion& rhs)
{
	Quaternion result = {
		this->x * rhs.w + this->w * rhs.x - this->z * rhs.y + this->y * rhs.z,
		this->y * rhs.w + this->z * rhs.x + this->w * rhs.y - this->x * rhs.z,
		this->z * rhs.w - this->y * rhs.x + this->x * rhs.y + this->w * rhs.z,
		this->w * rhs.w - this->x * rhs.x - this->y * rhs.y - this->z * rhs.z
	};

	return result;
}
//任意軸回転を表すQuaternionの生成
Quaternion MakeAxisAngle(const Vector3& axis, float angle)
{
	Vector3 vector = axis;
	vector = vector.normalize();
	Quaternion result = {
		vector.x * (float)sin(angle),
		vector.y * (float)sin(angle),
		vector.z * (float)sin(angle),
		angle
	};

	return result;
}
//ベクトルをQuaternionで回転させた結果のベクトルを求める
Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Vector3 vector_ = vector;

	vector_ = vector_.normalize();

	Quaternion p = { vector_.x,vector_.y,vector_.z,0 };

	Quaternion Q = {
		vector_.x * sin(quaternion.w / 2),
		vector_.y * sin(quaternion.w / 2),
		vector_.z * sin(quaternion.w / 2),
		cos(quaternion.w / 2)
	};

	Quaternion R = {
		-vector_.x * sin(quaternion.w / 2),
		-vector_.y * sin(quaternion.w / 2),
		-vector_.z * sin(quaternion.w / 2),
		cos(quaternion.w / 2)
	};

	R = R.Multiply(p);

	R = R.Multiply(Q);
	

	return Vector3(R.x, R.y, R.z);
}

Vector3 TransformAffine(const Vector3& vector, DirectX::XMMATRIX matrix) {
	float w =
		vector.x * matrix.r[0].m128_f32[3] +
		vector.y * matrix.r[1].m128_f32[3] + 
		vector.z * matrix.r[2].m128_f32[3] +
		matrix.r[3].m128_f32[3];

	Vector3 result
	{
		(vector.x * matrix.r[0].m128_f32[0] + vector.y * matrix.r[1].m128_f32[0] + vector.z * matrix.r[2].m128_f32[0] + matrix.r[3].m128_f32[0] * 0.0f) / w,
		(vector.x * matrix.r[0].m128_f32[1] + vector.y * matrix.r[1].m128_f32[1] + vector.z * matrix.r[2].m128_f32[1] + matrix.r[3].m128_f32[1] * 0.0f) / w,
		(vector.x * matrix.r[0].m128_f32[2] + vector.y * matrix.r[1].m128_f32[2] + vector.z * matrix.r[2].m128_f32[2] + matrix.r[3].m128_f32[2] * 0.0f) / w
	};

	return result;

	/*float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	NVector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0] * 0.0f) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1] * 0.0f) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2] * 0.0f) / w
	};*/
}


