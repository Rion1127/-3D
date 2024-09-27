#pragma once
#include "Human/HumanManager.h"
#include "Player/PlayerManager.h"

class Tutorial
{
	//シングルトン
private:
	Tutorial() = default;
	~Tutorial() = default;
public:
	//コピーコンストラクタ無効
	Tutorial(const Tutorial& obj) = delete;
	//代入演算子を無効
	Tutorial& operator=(const Tutorial& obj) = delete;

	static Tutorial* GetInstance();

public:
	void Update();
	void Draw();

	void InitTutorial() {
		isTutorial_ = false;
	}

	bool GetIsTutorial() {
		return isTutorial_;
	}
	int32_t GetTutorialNum() {
		return tutorialNum_;
	}

private:
	int32_t tutorialNum_ = 0;
	bool isTutorial_ = false;

	const int32_t SKIP_TIME = 300;
	int32_t skipTimer_;

	Sprite tutorialSp_;
	Sprite skipSp_;
	Sprite timerSp_;
};

