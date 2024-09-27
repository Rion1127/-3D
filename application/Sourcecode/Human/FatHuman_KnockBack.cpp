#include "Fathuman_KnockBack.h"
#include "PlayerManager.h"

void DebuHuman_KnockBack::SetUp()
{
	human_->SetState(HumanState::KNOCKBACK);
	human_->drawer_.SetAnimationTimer(90);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(false);

	
	if (human_->knockBack_.GetGravity() <= 0) {
		//吹き飛ばす処理（デブがロープにつかまってぶつかった場合）
		Vector3 knockVec = human_->pos_ - PlayerManager::GetInstance()->GetRopeEndPos();
		knockVec = knockVec.normalize();
		knockVec.y = 1.0f;
		human_->knockBack_.SetKnockVeclocity(knockVec * 0.7f);
	}
	SoundManager::Play("KnockbackSE",false,0.5f);
}

void DebuHuman_KnockBack::Update()
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

void DebuHuman_KnockBack::Draw()
{
}

void DebuHuman_KnockBack::CleanUp()
{
}
