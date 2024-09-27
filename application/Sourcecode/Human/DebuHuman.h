#pragma once
#include "BaseHuman.h"
#include "ColliderManager.h"
#include "GameStatus.h"
#include "FatHumanDrawer.h"

#include "FatHuman_Idle.h"
#include "FatHuman_Chase.h"
#include "FatHuman_Catch.h"
#include "FatHuman_Spawn.h"
#include "Fathuman_KnockBack.h"
#include "FatHuman_WakeUp.h"

class DebuHuman_Idle;
class DebuHuman_Chase;
class DebuHuman_Catch;
class DebuHuman_Spawn;
class DebuHuman_KnockBack;
class DebuHuman_WakeUp;

class DebuHuman :
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
public:
	//friendクラス先でプライベート変数にアクセスできるようにする
	friend DebuHuman_Idle;
	friend DebuHuman_Chase;
	friend DebuHuman_Catch;
	friend DebuHuman_Spawn;
	friend DebuHuman_KnockBack;
	friend DebuHuman_WakeUp;
private:
	// ステートマシンを初期化する。
	void InitStateMachine() override;
	DebuHumanDrawer drawer_;
};

