#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class RailCamera
{
public:
	void Ini(Vector3 pos, Vector3 rot);

	void Update();

	//�r���[�v���W�F�N�V����
	ViewProjection viewProjection;
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform;

};

