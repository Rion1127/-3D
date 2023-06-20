#pragma once

class Timer {
private:
	int timer_;

	int endTime_;

	bool isEnd_;
public:
	Timer();

	void AddTime(int time);
	void SetEndTime(int endTime);
	

};