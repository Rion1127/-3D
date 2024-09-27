#include "NormalHuman_Chase.h"
#include "PlayerManager.h"

void NormalHuman_Chase::SetUp()
{
	human_->SetState(HumanState::CHASE);
	human_->drawer_.SetAnimationTimer(20);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(true);
}

void NormalHuman_Chase::Update()
{
	human_->catchRopeSerial_ = PlayerManager::GetInstance()->GetNearRopeSerial(human_->pos_);
	//糸をつかむ
	Vector3 temp3D = PlayerManager::GetInstance()->GetNearRope(human_->pos_) - human_->pos_;
	if (temp3D.length() < human_->radius_ + PlayerManager::GetInstance()->GetCatchRadius()) {
		if (human_->catchRopeSerial_ == 0) {
			human_->CatchRope();
		}
		else {
			human_->isCatch_ = true;
			human_->catchNum_ = 0;
		}
		
		human_->GoToState(HumanState::CATCH);
		//コライダーにフラグを格納
		human_->collider_.Data_Remove("isCatch");
		human_->collider_.Data_Add("isCatch", human_->isCatch_);

		SoundManager::Play("GrabSE");
	}

	//追いかけている時の移動ベクトル更新
	Vector2 temp2D = {
		PlayerManager::GetInstance()->GetNearRope(human_->pos_).x - human_->pos_.x,
		PlayerManager::GetInstance()->GetNearRope(human_->pos_).z - human_->pos_.z
	};
	human_->moveVec_ = temp2D.normalize();
	//移動
	if (human_->isSearch_) {
		human_->pos_.x += human_->moveVec_.x * human_->speed_;
		human_->pos_.y -= 0.2f;
		human_->pos_.z += human_->moveVec_.y * human_->speed_;
	}

	if (human_->groupSerial_ != -1 && human_->isCatch_) {
		//コライダーにフラグを格納
		human_->collider_.Data_Remove("isCatch");
		human_->collider_.Data_Add("isCatch", human_->isCatch_);

		human_->CatchRope();

		human_->GoToState(HumanState::CATCH);
	}

	if (temp2D.length() < human_->searchRadius_ * 2 && (PlayerManager::GetInstance()->GetIsUse() || PlayerManager::GetInstance()->GetNearRopeSerial(human_->pos_) != 0)) {

	}
	//範囲外に出たら追いかけるのをやめる
	else if (human_->groupSerial_ == -1) {
		human_->isSearch_ = false;
		human_->GoToState(HumanState::IDLE);
	}
}

void NormalHuman_Chase::Draw()
{
}

void NormalHuman_Chase::CleanUp()
{
}
