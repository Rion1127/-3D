#include "FatHuman_WakeUp.h"

void DebuHuman_WakeUp::SetUp()
{
	int32_t wakeUpTimer = 90;
	human_->SetState(HumanState::WAKEUP);
	human_->drawer_.SetAnimationTimer(wakeUpTimer);
	human_->drawer_.SetPos(human_->pos_);
	human_->drawer_.SetRepeatAnimation(false);

	wakeUpTimer_.SetLimitTime(wakeUpTimer);
}

void DebuHuman_WakeUp::Update()
{
	wakeUpTimer_.AddTime();
	if (wakeUpTimer_.GetIsEnd())
	{
		human_->GoToState(HumanState::IDLE);
	}
}

void DebuHuman_WakeUp::Draw()
{
}

void DebuHuman_WakeUp::CleanUp()
{
	wakeUpTimer_.Reset();
}
