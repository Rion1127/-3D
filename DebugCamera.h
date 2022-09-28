#pragma once
#include "Vector3.h"
#include "DirectXInput.h"
#include "ViewProjection.h"
#include <DirectXMath.h>
using namespace DirectX;
class DebugCamera
{
public:
	void DebugCameraIni(HWND* hwnd);
	void Update(HWND hwnd);


	ViewProjection GetViewProjection();

private:
	MouseInput* mInput_ = nullptr;

	void CameraMove();

	ViewProjection viewProjection;

	Vector3 dist{};
	Vector3 moveDist{};	//���ʍ��W
	Vector3 cameraTrans{};//���s�ړ����W

	//���ʁE���E��x�N�g��
	Vector3 frontVec{};
	Vector3 sideVec{};
	Vector3 upVec{};

	

	float frontdist;

};

