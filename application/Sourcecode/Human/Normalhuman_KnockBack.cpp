#include "Normalhuman_KnockBack.h"
#include "mSound.h"

void Normalhuman_KnockBack::SetUp()
{
	human_->SetState(HumanState::KNOCKBACK);
	human_->drawer_.SetAnimationTimer(90);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(false);
	if (human_->isLeader_)
	{
		human_->isLeader_ = false;
		human_->isRingCrush_ = true;
	}
	human_->groupSerial_ = -1;
	SoundManager::Play("KnockbackSE", false, 0.5f);
}

void Normalhuman_KnockBack::Update()
{
	//ノックバック
	if (human_->isCatch_ == false)human_->pos_ += human_->knockBack_.GetKnockVelocity();
	human_->knockBack_.Update();
	if (human_->pos_.y < 0)
	{
		if (human_->knockBack_.GetGravity() < 0)
		{
			human_->knockBack_.GravityReset();
			human_->pos_.y += 1.f;
			human_->GoToState(HumanState::WAKEUP);
		}
	}
}

void Normalhuman_KnockBack::Draw()
{
}

void Normalhuman_KnockBack::CleanUp()
{
}
