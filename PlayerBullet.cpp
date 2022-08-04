#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Object3d.h"
#include "Texture.h"
#include "DirectXInput.h"
#include "math.h"
#include "Player.h"

PlayerBullet::PlayerBullet()
{
	
}

void PlayerBullet::Ini(ID3D12Device* device,WorldTransform worldTransform,ViewProjection viewProjection)
{

	worldTransform_.InitializeObject3d(device);

	cInput = Controller::GetInstance();
	cInput->Ini();

	texture = TextureManager::GetInstance()->LoadGraph("Resources/enemy.jpg");

	worldTransform_.position = worldTransform.position;
	worldTransform_.rotation = worldTransform.rotation;
	worldTransform_.SetScale(0.1f, 0.1f, 2);
	worldTransform_.UpdateObject3d(viewProjection);
}

void PlayerBullet::Update(ViewProjection viewProjection)
{

	Move();

	worldTransform_.UpdateObject3d(viewProjection);
}

void PlayerBullet::Draw()
{
	model_.Draw(&worldTransform_, texture);


}

void PlayerBullet::Move()
{
	
}


