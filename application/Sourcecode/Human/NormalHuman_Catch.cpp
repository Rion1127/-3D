#include "NormalHuman_Catch.h"
#include "PlayerManager.h"
#include "RRandom.h"

void NormalHuman_Catch::SetUp()
{
	human_->SetState(HumanState::CATCH);
	human_->drawer_.SetAnimationTimer(20);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(false);
	if (human_->catchRopeSerial_ == 0) {
		PlayerManager::GetInstance()->AddCatchCount();
	}
	PlayerManager::GetInstance()->RegisterCatchHuman(human_);
	human_->collider_.Set_IsActive(false);
}

void NormalHuman_Catch::Update()
{
	if (human_->isCatch_) {
		human_->pos_ = PlayerManager::GetInstance()->GetSerialRopePos(human_->catchRopeSerial_, 9 - human_->catchNum_ * 2);
		PlayerManager::GetInstance()->SetIsCatch(true, human_->catchRopeSerial_);
	}
	if (human_->isCatch_ == false && human_->catchRopeSerial_ == 0)
	{
		float valueX = RRandom::RandF(-1.f, 1.f);
		float valueY = RRandom::RandF(0.7f, 1.f);
		float valueZ = RRandom::RandF(-1.f, 1.f);
		Vector3 knockVec = { valueX ,valueY ,valueZ };
		knockVec = knockVec.normalize();
		knockVec.y = 1.0f;
		human_->knockBack_.SetKnockVeclocity(knockVec);
		human_->GoToState(HumanState::KNOCKBACK);
	}
}

void NormalHuman_Catch::Draw()
{
}

void NormalHuman_Catch::CleanUp()
{
	PlayerManager::GetInstance()->UnRegisterCatchHuman(human_);
	human_->collider_.Set_IsActive(true);
}
