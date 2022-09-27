

#include <cmath>
#include "Enemy.h"

void Enemy::Ini()
{
	worldTransoform.InitializeObject3d();
	texture = TextureManager::GetInstance()->LoadGraph("Resources/enemy.jpg");

	worldTransoform.SetPosition(0, 0, 20);
	worldTransoform.SetRotation(0, 0, 0);
	worldTransoform.SetScale(1, 1, 1);

	//�t�F�[�Y������
	phase_ = Phase::Approach;
	isAlive = true;
}

void Enemy::Update(ViewProjection viewprojection)
{
	//Move();

	worldTransoform.UpdateObject3d(viewprojection);

	if (HP <= 0) {
		isAlive = false;
	}
}

void Enemy::OnCollision()
{
	HP--;
}

void Enemy::Draw()
{
	if (isAlive == true) {
		model_.DrawCube(&worldTransoform, texture);
	}
}

void Enemy::Move()
{
	switch (phase_) {
		//�߂Â��Ă���
	case Phase::Approach:
	default:
		//��O�ɋ߂Â��Ă���
		Approach();
		break;

		//���E�t�F�[�Y
	case Phase::Leave:
		//�ړ�(�x�N�g�������Z)
		Leave();
		break;
	}
}
//�߂Â��Ă���
void Enemy::Approach()
{
	//��O�ɋ߂Â��Ă���
	worldTransoform.AddPosition(0, 0, -0.01f);
	//����̈ʒu�ɓ��B������ʂ̃t�F�[�Y�ֈڍs����
	if (worldTransoform.position.z < 5.0f) {
		phase_ = Phase::Leave;
	}
}
//�����
void Enemy::Leave()
{
	//�ړ�(�x�N�g�������Z)
	worldTransoform.AddPosition(-0.2f, 0, 0);
}
