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
	Vector3 moveDist_{};	//���ʍ��W
	Vector3 cameraTrans_{};//���s�ړ����W

	//���ʁE���E��x�N�g��
	Vector3 frontVec_{};
	Vector3 sideVec_{};
	Vector3 upVec_{};

	

	float frontdist_;

};

