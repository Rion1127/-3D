#pragma once
#include "Vector3.h"
#include "mInput.h"
#include "Camera.h"
#include <DirectXMath.h>
using namespace DirectX;
class DebugCamera
{
public:
	void DebugCameraIni();
	void Update();


	Camera* GetViewProjection();

private:
	MouseInput* mInput_ = nullptr;

	void CameraMove();

	Camera viewProjection_;

	Vector3 dist_{};
	Vector3 moveDist_{};	//球面座標
	Vector3 cameraTrans_{};//平行移動座標

	//正面・横・上ベクトル
	Vector3 frontVec_{};
	Vector3 sideVec_{};
	Vector3 upVec_{};

	

	float frontdist_;

};

