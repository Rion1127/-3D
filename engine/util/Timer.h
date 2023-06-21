#pragma once
#include <stdint.h>

class Timer {
private:
	int32_t timer_;

	int32_t limitTime_;

	bool isEnd_;
public:
	Timer();

	//�J�E���g�A�b�v
	void AddTime(int32_t addValue);
	//�J�E���g�_�E��
	void SubTime(int32_t addValue);
public:
	//�Z�b�^�[
	void SetLimitTime(int32_t limitTime) { limitTime_ = limitTime; }
	void SetTime(int32_t setValue) { timer_ = setValue; }
	void SetIsEnd(bool flag) { isEnd_ = flag; }
public:
	//�Q�b�^�[
	int32_t GetTimer() { return timer_; }
	int32_t GetLimitTimer() { return limitTime_; }
	bool GetIsEnd() { return isEnd_; }
};