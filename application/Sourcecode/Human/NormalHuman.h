#pragma once
#include "BaseHuman.h"
#include "ColliderManager.h"
#include "GameStatus.h"
#include "HumanDrawer.h"

#include "NormalHuman_Idle.h"
#include "NormalHuman_Chase.h"
#include "NormalHuman_Catch.h"
#include "NormalHuman_Spawn.h"
#include "Normalhuman_KnockBack.h"
#include "NormalHuman_WakeUp.h"

class NormalHuman_Idle;
class NormalHuman_Chase;
class NormalHuman_Catch;
class NormalHuman_Spawn;
class Normalhuman_KnockBack;
class NormalHuman_WakeUp;

class NormalHuman :
	public BaseHuman, public StateMachine<HumanState>
{
public:
	void Initialize(
		Vector2 pos,
		Vector2 vec,
		const std::string& colName,
		ColliderManager* colManagerPtr,
		HumanState popState = HumanState::SPAWN,
		int32_t group = -1,
		bool isLeader = false) override;
	void Update() override;
	void Draw() override;
	void CallBack() override;
	void Knock();
private:
	//人同士の押し戻し
	void PushBack(const std::string& colName);
public:
	//friendクラス先でプライベート変数にアクセスできるようにする
	friend NormalHuman_Idle;
	friend NormalHuman_Chase;
	friend NormalHuman_Catch;
	friend NormalHuman_Spawn;
	friend Normalhuman_KnockBack;
	friend NormalHuman_WakeUp;
private:
	// ステートマシンを初期化する。
	void InitStateMachine() override;
	HumanDrawer drawer_;
};

