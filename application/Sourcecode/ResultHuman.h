#pragma once
#include "Human/FatHumanDrawer.h"
#include "Human/HumanDrawer.h"
class ResultHuman
{
public:
	ResultHuman() = default;
	~ResultHuman() = default;

	void Init();
	void Update();
	void Draw();
private:
	void PopHuman();
private:
	HumanState state_;
	std::vector<std::unique_ptr<HumanDrawer>> normal_human_objects_;
	std::vector<std::unique_ptr<DebuHumanDrawer>> debu_human_objects_;

	std::vector<HumanType> helpHumanType_;
};

