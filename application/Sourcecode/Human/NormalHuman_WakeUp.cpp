#include "NormalHuman_WakeUp.h"

void NormalHuman_WakeUp::SetUp()
{
	int32_t wakeUpTimer = 90;
	human_->SetState(HumanState::WAKEUP);
	human_->drawer_.SetAnimationTimer(wakeUpTimer);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(false);

	wakeUpTimer_.SetLimitTime(wakeUpTimer);
}

void NormalHuman_WakeUp::Update()
{
	wakeUpTimer_.AddTime();
	if (wakeUpTimer_.GetIsEnd())
	{
		human_->GoToState(HumanState::IDLE);
	}
}

void NormalHuman_WakeUp::Draw()
{
}

void NormalHuman_WakeUp::CleanUp()
{
	wakeUpTimer_.Reset();
}
