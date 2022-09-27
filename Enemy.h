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
	void Ini();
	void Update(ViewProjection viewprojection);

	//�����蔻������o������Ăяo�����֐�
	void OnCollision();

	void Draw();
	WorldTransform GetWorldTransform() { return worldTransoform; }
	Vector3 GetPosition() { return worldTransoform.position; }
	bool GetAlive() { return isAlive; }
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

	const int maxHP = 10;
	int HP = maxHP;
	bool isAlive = false;

};

