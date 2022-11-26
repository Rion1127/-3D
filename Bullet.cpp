#include "Bullet.h"

Bullet::~Bullet()
{
	delete object_;
}

void Bullet::Ini()
{
	// 弾オブジェクト
	object_ = Object3d::CreateOBJ("net");
	worldTransform_.Ini();

	// 弾を飛ばす方向
	moveVec_ = { 0.0f, 0.0f, 0.0f };

	// 弾の速度
	speed_ = 5.0f;

	// スケールに加算する値
	addScale_ = 0.1f;

	// 弾が生存しているかフラグ
	isActive_ = false;
}

void Bullet::Update(ViewProjection viewProjection)
{
	// フラグが[ON]なら
	if (isActive_) {
		// 弾の移動処理
		worldTransform_.position += moveVec_;

		// スケールの値の変更
		scale_ += addScale_;
		worldTransform_.scale = { scale_, scale_, 1.0f };

		// 行列の再計算
		worldTransform_.Update(viewProjection);
	}
}

void Bullet::Draw()
{
	// フラグが[ON]なら描画する
	if (isActive_) object_->DrawOBJ(&worldTransform_);
}

void Bullet::Reset() {
	// スケールをもとの大きさに戻す
	scale_ = 1.0f;
}