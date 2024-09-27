#include "NormalHuman_Idle.h"
#include "PlayerManager.h"
#include "HumanManager.h"
#include "Tutorial.h"

void NormalHuman_Idle::SetUp()
{
	human_->SetState(HumanState::IDLE);
	human_->drawer_.SetAnimationTimer(120);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(true);
}

void NormalHuman_Idle::Update()
{
	Vector2 temp2D = {
		PlayerManager::GetInstance()->GetNearRope(human_->pos_).x - human_->pos_.x,
		PlayerManager::GetInstance()->GetNearRope(human_->pos_).z - human_->pos_.z
	};
	
	if (temp2D.length() < human_->searchRadius_ * 2 && (PlayerManager::GetInstance()->GetIsUse() || PlayerManager::GetInstance()->GetNearRopeSerial(human_->pos_) != 0) && !(Tutorial::GetInstance()->GetTutorialNum() > 3 && Tutorial::GetInstance()->GetTutorialNum() < 6)) {
		human_->isSearch_ = true;
		if (human_->groupSerial_ != -1) {
			HumanManager::GetInstance()->SetSearchGroup(human_->groupSerial_,true);
		}
		human_->GoToState(HumanState::CHASE);
		human_->moveVec_ = temp2D.normalize();
	}
	else if(human_->groupSerial_ == -1){
		human_->isSearch_ = false;
	}

	if (human_->isSearch_) {
		human_->GoToState(HumanState::CHASE);
		human_->moveVec_ = temp2D.normalize();
	}
}

void NormalHuman_Idle::Draw()
{
}

void NormalHuman_Idle::CleanUp()
{
	human_->drawer_.SetPos(human_->pos_);
}
