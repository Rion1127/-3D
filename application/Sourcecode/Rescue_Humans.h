#pragma once
#include <vector>
#include "GameStatus.h"
#include "Human/FatHumanDrawer.h"
#include "Human/HumanDrawer.h"
#include "Sprite.h"
class Rescue_Humans
{
	//シングルトン
private:
	Rescue_Humans() = default;
	~Rescue_Humans() = default;
public:
	//コピーコンストラクタ無効
	Rescue_Humans(const Rescue_Humans& obj) = delete;
	//代入演算子を無効
	Rescue_Humans& operator=(const Rescue_Humans& obj) = delete;

	static Rescue_Humans* GetInstance();
public:
	void Init();
	void Update();
	void Draw();
	void DrawSprite();
	void DrawFrontSprite();

	void PopHuman(HumanType type);
	void SetIsFever(bool flag) { isFever_ = flag; };
private:
	void PopHelpHuman();
private:
	struct WaitPoint {
		bool isActive = true;
		Vector3 pos;
	};
	enum RescueHumanState {
		NONE_FIVER,
		FIVER_FLOAT,	//背景の人が上に飛ぶ
		FIVER_FALL,		//でかい雲のうえに落ちる
		FIVER_HELP		//ｿｰﾗﾝｿｰﾗﾝ
	};
private:
	HumanState state_;
	std::vector<std::unique_ptr<HumanDrawer>> normal_human_objects_;
	std::vector<std::unique_ptr<DebuHumanDrawer>> debu_human_objects_;

	std::vector<WaitPoint> point_;
	std::vector<WaitPoint> feverpoint_;
	bool isFever_;

	float rescueCloud_fever_y;
	float rescueCloud_none_fever_y;
	Sprite rescueCloudLeft_;
	Sprite rescueCloudRight_;

	float bigCloud_fever_y;
	float bigCloud_none_fever_y;
	Sprite bigCloud_;
	RescueHumanState rescueHumanstate_;
};

