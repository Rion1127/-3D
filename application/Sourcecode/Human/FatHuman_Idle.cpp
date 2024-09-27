#include "FatHuman_Idle.h"
#include "PlayerManager.h"

void DebuHuman_Idle::SetUp()
{
	human_->SetState(HumanState::IDLE);
	human_->drawer_.SetAnimationTimer(120);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(true);
}

void DebuHuman_Idle::Update()
{
	Vector2 temp2D = {
		PlayerManager::GetInstance()->GetNearRope(human_->pos_).x -
		human_->pos_.x,PlayerManager::GetInstance()->GetNearRope(human_->pos_).z -
		human_->pos_.z
	};
	
	if (temp2D.length() < human_->searchRadius_ * 2 && (PlayerManager::GetInstance()->GetIsUse() || PlayerManager::GetInstance()->GetNearRopeSerial(human_->pos_) != 0)) {
		human_->isSearch_ = true;
		human_->GoToState(HumanState::CHASE);
		human_->moveVec_ = temp2D.normalize();
	}
	else {
		human_->isSearch_ = false;
	}
}

void DebuHuman_Idle::Draw()
{
}

void DebuHuman_Idle::CleanUp()
{
	human_->drawer_.SetPos(human_->pos_);
}
