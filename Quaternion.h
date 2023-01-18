#pragma once
#include <DirectXMath.h>
struct Quaternion
{
	
	float x;
	float y;
	float z;
	float w;

	DirectX::XMMATRIX UpdateMatrix() {
		Quaternion q1 = {
		(float)(x * sin(w / 2)),
		(float)(y * sin(w / 2)),
		(float)(z * sin(w / 2)),
		(float)(cos(w / 2))
		};

		DirectX::XMVECTOR vectorX = {
			(float)(pow(q1.w,2) + pow(q1.x,2) - pow(q1.y,2) - pow(q1.z,2)),
			2 * (q1.x * q1.y + q1.w * q1.z),
			2 * (q1.x * q1.z - q1.w * q1.y),
			0
		};
		DirectX::XMVECTOR vectorY = {
			2 * (q1.x * q1.y - q1.w * q1.z),
			(float)(pow(q1.w,2) - pow(q1.x,2) + pow(q1.y,2) - pow(q1.z,2)),
			2 * (q1.y * q1.z + q1.w * q1.x),
			0
		};
		DirectX::XMVECTOR vectorZ = {
			2 * (q1.x * q1.z + q1.w * q1.y),
			2 * (q1.y * q1.z - q1.w * q1.x),
			(float)(pow(q1.w,2) - pow(q1.x,2) - pow(q1.y,2) + pow(q1.z,2)),
			0
		};
		DirectX::XMVECTOR vectorW = {
			0,0,0,1
		};

		DirectX::XMMATRIX matRot;

		matRot.r[0] = vectorX;
		matRot.r[1] = vectorY;
		matRot.r[2] = vectorZ;
		matRot.r[3] = vectorW;

		return matRot;
	};
	//íPà QuaternionÇï‘Ç∑
	Quaternion IdentityQuaternion();
	//ã§ñQuaternionÇï‘Ç∑
	Quaternion Conjugate();
	//QuaternionÇÃnormÇï‘Ç∑
	float Norm();
	//ê≥ãKâªÇµÇΩQuaternionÇï‘Ç∑
	Quaternion Normalize();
	//ãtQuaternionÇï‘Ç∑
	Quaternion Inverse(const Quaternion& quaternion);
	//QuaternionÇÃêœ 
	Quaternion Multiply(const Quaternion& rhs);
};

