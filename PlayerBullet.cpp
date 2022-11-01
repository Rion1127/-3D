
#include "Player.h"

PlayerBullet::PlayerBullet()
{
	
}

void PlayerBullet::Ini(WorldTransform worldTransform,Vector3 velocity, Object3d* model)
{
	model_ = model;

	worldTransform_.Initialize();

	cInput = Controller::GetInstance();
	cInput->Ini();

	texture = TextureManager::GetInstance()->LoadGraph("Resources/bullet3.png");

	worldTransform_.position = worldTransform.position;
	worldTransform_.rotation = worldTransform.rotation;
	worldTransform_.SetScale(0.1f, 0.01f, 2);

	velocity_ = velocity;
}

void PlayerBullet::Update(ViewProjection viewProjection)
{
	Move();
	worldTransform_.UpdateObject3d(viewProjection);

	//åoâﬂéûä‘Ç≈è¡Ç¶ÇÈ
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

void PlayerBullet::Draw()
{
	model_->DrawCube(&worldTransform_, texture);


}

void PlayerBullet::Move()
{
	worldTransform_.AddPosition(velocity_.x, velocity_.y, velocity_.z);
}


