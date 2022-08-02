#pragma once
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "math.h"


class ViewProjection {
public:
	ViewProjection();
	//�J�������W
	void SetEyePos(float x,float y,float z);
	void SetEyePos(Vector3 pos);
	//�^�[�Q�b�g�Z�b�g
	void SetTarget(float x, float y, float z);
	void SetTarget(Vector3 pos);
	//�A�b�v�x�N�g��
	void SetUpVec(float x, float y, float z);
	void SetUpVec(Vector3 upVec);
	//������
	void Ini();
	void Update();

	XMMATRIX GetMatView();
	XMMATRIX GetMatProjection();
	XMFLOAT3 eye;
	XMFLOAT3 target;
	XMFLOAT3 up;

	XMMATRIX matView{};
	//�������e�s��̌v�Z
	XMMATRIX matProjection{};
private:

	// �r���[�|�[�g�̃A�X�y�N�g��
	const float aspectRatio = (float)16 / 9;

};

