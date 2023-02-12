#pragma once
#include <DirectXMath.h>
#include "Vector3.h"
struct IParticle
{
	//座標
	Vector3 position = {};
	//速度
	Vector3 velocity = {};
	//現在フレーム
	int frame = 0;
	//終了フレーム
	int end_frame = 0;
	//スケール
	float scale = 1.0f;
	
	DirectX::XMFLOAT4 color_;
};

