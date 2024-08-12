#include "HitStop.h"

HitStop* HitStop::GetInstance()
{
	static HitStop instance;
	return &instance;
}

HitStop::HitStop()
{
	timer_.SetLimitTime(0);
}

void HitStop::Update()
{
	timer_.AddTime();
}


void HitStop::SetIsHitStop(int32_t time)
{
	timer_.SetLimitTime(time);
	timer_.Reset();
}
