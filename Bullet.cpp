#include "Bullet.h"

Bullet::~Bullet()
{
	delete object_;
}

void Bullet::Ini()
{
	// �e�I�u�W�F�N�g
	object_ = Object3d::CreateOBJ("net");
	worldTransform_.Ini();

	// �e���΂�����
	moveVec_ = { 0.0f, 0.0f, 0.0f };

	// �e�̑��x
	speed_ = 5.0f;

	// �X�P�[���ɉ��Z����l
	addScale_ = 0.1f;

	// �e���������Ă��邩�t���O
	isActive_ = false;
}

void Bullet::Update(ViewProjection viewProjection)
{
	// �t���O��[ON]�Ȃ�
	if (isActive_) {
		// �e�̈ړ�����
		worldTransform_.position += moveVec_;

		// �X�P�[���̒l�̕ύX
		scale_ += addScale_;
		worldTransform_.scale = { scale_, scale_, 1.0f };

		// �s��̍Čv�Z
		worldTransform_.Update(viewProjection);
	}
}

void Bullet::Draw()
{
	// �t���O��[ON]�Ȃ�`�悷��
	if (isActive_) object_->DrawOBJ(&worldTransform_);
}

void Bullet::Reset() {
	// �X�P�[�������Ƃ̑傫���ɖ߂�
	scale_ = 1.0f;
}