#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Object3d.h"
#include "Texture.h"
#include "DirectXInput.h"
#include "math.h"
class PlayerBullet
{
public:
	PlayerBullet();

	void Ini(ID3D12Device* device, WorldTransform worldTransform, Vector3 velocity, Object3d* model);

	void Update(ViewProjection viewProjection);
	//�����蔻������o������Ăяo�����֐�
	void OnCollision();

	bool IsDead() const { return isDead_; }
	XMFLOAT3 GetPosition() { return worldTransform_.position; }

	void Draw();
private:
	void Move();

	WorldTransform worldTransform_;
	Object3d* model_ = nullptr;

	Vector3 velocity_;

	Controller* cInput;

	uint32_t texture;

	//����
	static const int32_t lifeTimer = 60 * 5;
	int32_t deathTimer_ = lifeTimer;
	//�f�X�t���O
	bool isDead_ = false;

};

