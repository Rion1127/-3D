#include "Player.h"
#include "mInput.h"
#include "ModelManager.h"

void Player::Initialize()
{
	//3Dオブジェクト
	playerObj_ = std::move(std::make_unique<Object3d>());
	//モデル読み込み
	playerObj_->SetModel(ModelManager::GetInstance()->GetModel("Hand"));

	playerObj_->SetScale({5,5,5});
	//糸のたらし始め
	pos_ = { -6.f,PLAYER_Y,4.f };
	offsetPos_ = pos_;
	offsetPos_.x += 6.f;
	offsetPos_.y += -1.f;
	offsetPos_.z += -4.f;

	playerObj_->SetRot({ 0.f,Radian(180.f) ,0.f });
}

void Player::Update()
{
	float speed = SPEED;
	if (isFever_) speed *= 2.f;
	pos_.x += (Controller::GetLStick().x + Key::PushKey(DIK_D) - Key::PushKey(DIK_A)) * speed;
	pos_.z += (Controller::GetLStick().y + Key::PushKey(DIK_W) - Key::PushKey(DIK_S)) * speed;

	if (pos_.length() > MOVE_RADIUSE) {
		pos_ = pos_.normalize() * MOVE_RADIUSE;
		pos_.y = PLAYER_Y;
	}

	offsetPos_ = pos_;
	offsetPos_.x += 6.f;
	offsetPos_.y += -1.f;
	offsetPos_.z += -4.f;

	//オブジェクトの更新
	playerObj_->SetPos({ pos_.x,pos_.y + 2 ,pos_.z});
	playerObj_->Update();
}

void Player::Draw()
{
	//オブジェクトの描画
	playerObj_->Draw();
}
