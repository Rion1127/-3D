#pragma once
#include <vector>
#include <array>
#include "Sprite.h"
#include "RescueCount.h"
#include "Easing.h"
#include "Timer.h"
class Ranking
{
private:
	Ranking() {};
	~Ranking() = default;

public:
	static Ranking* GetInstance(void);
	//初期化
	void Init();
	//更新
	void Update();
	//描画
	void Draw();

	void Reset();
	void EasingSkip();
public:
	void SetRanking(const std::vector<int32_t>& ranking) { ranking_ = ranking; }
	void SetNewScore(int32_t newScore) { newScore_ = newScore; }
public:
	int32_t GetNewScore() const { return newScore_; }
	int32_t GetEasingNewScore() const { return easingNewScore_; }
	Timer GetEasingTimer() const { return easingTimer_; }
private:
	std::vector<int32_t> ranking_;

	int32_t newScore_;
	int32_t easingNewScore_;
	Timer easingTimer_;

	std::array<RescueCount, 3> sprite_;
	std::array<Sprite, 4> peopleNumSprite_;

	RescueCount newScoreSprite_;
};

