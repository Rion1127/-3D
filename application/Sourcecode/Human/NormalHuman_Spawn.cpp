#include "NormalHuman_Spawn.h"
#include "HumanManager.h"

void NormalHuman_Spawn::SetUp()
{
	human_->SetState(HumanState::SPAWN);
	human_->drawer_.SetAnimationTimer(20);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(true);
}

void NormalHuman_Spawn::Update()
{
	if (--human_->startTimer_ > 0) {
		if (human_->groupSerial_ != -1 && !human_->isLeader_) {
			Vector3 temp = HumanManager::GetInstance()->GetLeaderPos(human_->groupSerial_) - human_->pos_;
			human_->moveVec_ = Vector2(temp.x, temp.z).normalize();
		}
		human_->pos_.x += human_->moveVec_.x;
		human_->pos_.z += human_->moveVec_.y;
	}
	if (human_->startTimer_ <= 0) {
		human_->GoToState(HumanState::IDLE);
	}
}

void NormalHuman_Spawn::Draw()
{
}

void NormalHuman_Spawn::CleanUp()
{
}
