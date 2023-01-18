#include "Quaternion.h"

//’PˆÊQuaternion‚ğ•Ô‚·
Quaternion Quaternion::IdentityQuaternion()
{
	Quaternion identity = { 0,0,0,1 };
	

	return identity;
}
//‹¤–ğQuaternion‚ğ•Ô‚·
Quaternion Quaternion::Conjugate()
{
	Quaternion result = {
	this->x *= -1,
	this->y *= -1,
	this->z *= -1,
	w
	};
	return result;
}
//Quaternion‚Ìnorm‚ğ•Ô‚·
float Quaternion::Norm()
{
	return sqrt(w * w + x * x + y * y + z * z);
}
//³‹K‰»‚µ‚½Quaternion‚ğ•Ô‚·
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
//‹tQuaternion‚ğ•Ô‚·
Quaternion Quaternion::Inverse(const Quaternion& quaternion)
{
	Quaternion result = {
		this->x / pow(Norm(),2),
		this->y / pow(Norm(),2),
		this->z / pow(Norm(),2),
		this->w / pow(Norm(),2),
	};

	return result;
}
//Quaternion‚ÌÏ
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
