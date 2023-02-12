#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
struct IParticle
{
	//���W
	Vector3 position = {};
	//���x
	Vector3 velocity = {};
	//���݃t���[��
	int frame = 0;
	//�I���t���[��
	int end_frame = 0;
	//�X�P�[��
	float scale = 1.0f;
	
	DirectX::XMFLOAT4 color_;
};

