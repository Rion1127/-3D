#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Object3d.h"
#include "Texture.h"
#include "math.h"
#include <cmath>
#include "Enemy.h"

void Enemy::Ini(ID3D12Device* device)
{
	worldTransoform.InitializeObject3d(device);
	texture = TextureManager::GetInstance()->LoadGraph("Resources/enemy.jpg");

	worldTransoform.SetPosition(10, 10, 100);
	worldTransoform.SetRotation(0, 0, 0);
	worldTransoform.SetScale(1, 1, 1);

	//フェーズ初期化
	phase_ = Phase::Approach;
}

void Enemy::Update(ViewProjection viewprojection)
{
	Move();

	worldTransoform.UpdateObject3d(viewprojection);
}

void Enemy::Draw()
{
	model_.Draw(&worldTransoform, texture);
}

void Enemy::Move()
{
	switch (phase_) {
		//近づいてくる
	case Phase::Approach:
	default:
		//手前に近づいてくる
		Approach();
		break;

		//離脱フェーズ
	case Phase::Leave:
		//移動(ベクトルを加算)
		Leave();
		break;
	}
}
//近づいてくる
void Enemy::Approach()
{
	//手前に近づいてくる
	worldTransoform.AddPosition(0, 0, -0.2f);
	//既定の位置に到達したら別のフェーズへ移行する
	if (worldTransoform.position.z < 5.0f) {
		phase_ = Phase::Leave;
	}
}
//離れる
void Enemy::Leave()
{
	//移動(ベクトルを加算)
	worldTransoform.AddPosition(-0.2f, 0, 0);
}
