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
		x * sin(w / 2),
		y * sin(w / 2),
		z * sin(w / 2),
		cos(w / 2)
		};

		DirectX::XMVECTOR vectorX = {
			pow(q1.w,2) + pow(q1.x,2) - pow(q1.y,2) - pow(q1.z,2),
			2 * (q1.x * q1.y + q1.w * q1.z),
			2 * (q1.x * q1.z - q1.w * q1.y),
			0
		};
		DirectX::XMVECTOR vectorY = {
			2 * (q1.x * q1.y - q1.w * q1.z),
			pow(q1.w,2) - pow(q1.x,2) + pow(q1.y,2) - pow(q1.z,2),
			2 * (q1.y * q1.z + q1.w * q1.x),
			0
		};
		DirectX::XMVECTOR vectorZ = {
			2 * (q1.x * q1.z + q1.w * q1.y),
			2 * (q1.y * q1.z - q1.w * q1.x),
			pow(q1.w,2) - pow(q1.x,2) - pow(q1.y,2) + pow(q1.z,2),
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
};

