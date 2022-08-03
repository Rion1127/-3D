#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Object3d.h"
#include "Texture.h"
#include "DirectXInput.h"
#include "Player.h"

Player::Player()
{
	worldTransform.SetPosition(0, 0, 0);
	worldTransform.SetRotation(0, 0, 0);
	worldTransform.SetScale(1, 1, 1);
}

void Player::Ini(ID3D12Device* device)
{
	worldTransform.InitializeObject3d(device);
	model_.Ini(device);

	cInput = Controller::GetInstance();
	cInput->Ini();
}

void Player::Update(ViewProjection viewProjection)
{

	worldTransform.UpdateObject3d(viewProjection);
}

void Player::Draw(uint32_t graph)
{
	model_.Draw(&worldTransform, graph);
}

void Player::Move()
{
}
