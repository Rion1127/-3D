#include "Timer.h"

Timer::Timer()
{
	timer_ = 0;
	endTime_ = 0;
	isEnd_ = false;
}

void Timer::AddTime(int time)
{
	timer_ += time;
	
	//if(timer_)
}

void Timer::SetEndTime(int endTime)
{
}
