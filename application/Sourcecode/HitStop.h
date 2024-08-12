#pragma once
#include "Timer.h"
class HitStop
{
public:
	HitStop();
	void Update();
public:
	static HitStop* GetInstance();
	bool GetIsHitStopping() { return !timer_.GetIsEnd(); }
public:
	void SetIsHitStop(int32_t time);
private:
	Timer timer_;
};

