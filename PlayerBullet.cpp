#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Object3d.h"
#include "Texture.h"
#include "DirectXInput.h"
#include "math.h"
#include "PlayerBullet.h"

PlayerBullet::PlayerBullet()
{
}

void PlayerBullet::Ini(ID3D12Device* device,WorldTransform worldTransform, Object3d* model)
{
	assert(model);
	worldTransform.InitializeObject3d(device);

	texture = TextureManager::GetInstance()->LoadGraph("Resources/keyBlade2.png");
	//座標のをセットする
	worldTransform.SetPosition(worldTransform.position.x, worldTransform.position.y, worldTransform.position.z);
	
}

void PlayerBullet::Update(ViewProjection viewProjection)
{	

	worldTransform.UpdateObject3d(viewProjection);
}

void PlayerBullet::Draw()
{
	model_->Draw(&worldTransform, texture);
}

void PlayerBullet::Move()
{
	
}

