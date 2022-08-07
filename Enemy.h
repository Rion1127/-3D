#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <d3d12.h>
#include "Object3d.h"
#include "Texture.h"
#include "math.h"
class Enemy
{
public:
	void Ini(ID3D12Device* device);
	void Update(ViewProjection viewprojection);

	//�����蔻������o������Ăяo�����֐�
	void OnCollision();

	void Draw();

	XMFLOAT3 GetPosition() { return worldTransoform.position; }
private:
	void Move();
	void Approach();
	void Leave();
	WorldTransform worldTransoform;
	Object3d model_{};

	uint32_t texture;

	//�s���t�F�[�Y
	enum class Phase{
		Approach,	//�ڋ߂���
		Leave,		//���E����
	};
	Phase phase_;

};

