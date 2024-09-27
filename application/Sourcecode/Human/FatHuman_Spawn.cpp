#include "FatHuman_Spawn.h"

void DebuHuman_Spawn::SetUp()
{
	human_->SetState(HumanState::SPAWN);
	human_->drawer_.SetAnimationTimer(20);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(true);
}

void DebuHuman_Spawn::Update()
{
	if (--human_->startTimer_ > 0) {
		human_->pos_.x += human_->moveVec_.x;
		human_->pos_.z += human_->moveVec_.y;
		
	}
	if (human_->startTimer_ <= 0) {
		human_->GoToState(HumanState::IDLE);
	}
}

void DebuHuman_Spawn::Draw()
{
}

void DebuHuman_Spawn::CleanUp()
{
}
