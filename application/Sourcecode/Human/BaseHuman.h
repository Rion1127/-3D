#pragma once
#include "Object3d.h"
#include "ColliderObject.h"
#include "KnockBack.h"
#include "Human//HumanDrawer.h"
#include "GameStatus.h"

class BaseHuman :
	public ColliderObject
{
public:
	BaseHuman() = default;
	/// <summary>
	/// デフォルトデストラクタ
	/// </summary>
	virtual ~BaseHuman();
	virtual void Initialize(
		Vector2 pos,
		Vector2 vec,
		const std::string& colName,
		ColliderManager* colManagerPtr,
		HumanState popState = HumanState::SPAWN, 
		int32_t groupNum = -1,
		bool isLeader = false);
	virtual void Update();
	virtual void Draw();
	virtual void Rescue();
	void SetDead();

	virtual void CallBack() = 0;

	bool GetIsDead() {
		return isDead_;
	}
	bool GetIsAddScore() {
		return isAddScore_;
	}

	bool GetIsCatch() {
		return isCatch_;
	}
	void SetIsCatch(bool is) {
  		isCatch_ = is;
	}
	void SetIsAddScore(bool is) {
		isAddScore_ = is;
	}
	int32_t GetWeight() {
		return weight_;
	}

	void SetState(HumanState state) { state_ = state; }
	HumanState GetState() {
		return state_;
	}
	HumanState* GetHumanState() { return &state_; }
	HumanType GetHumanType() { return humanType_; }
	void CatchRope();

	Vector3 GetPos() {
		return pos_;
	}
	std::string GetColName() { return collider_.Get_Name(); }

	int32_t GetGroupNum() {
		return groupSerial_;
	}
	bool GetIsLeader() {
		return isLeader_;
	}

	int32_t GetStartTimer() {
		return startTimer_;
	}

	void SetIsSearch(bool is) {
		isSearch_ = is;
	}

	void SetIsKnock() {
		isKnock_ = true;
	}
	bool GetIsKnock(){
		return isKnock_;
	}

	int32_t GetCatchRopeSerial() {
		return catchRopeSerial_;
	}

protected:
	const float MOVE_RADIUSE = 60.f;

	std::unique_ptr<Object3d> obj_;
	std::unique_ptr<Object3d> ling_;
	
	HumanType humanType_;
	HumanState state_;
	Vector3 pos_;
	Vector3 scale_;
	bool isDead_;
	bool isAddScore_;	//死んだときに得点を追加するかどうか
	float radius_;

	Vector2 moveVec_;
	bool isSearch_;

	float searchRadius_;
	float speed_;

	int32_t weight_;

	bool isCatch_;
	int32_t catchNum_;
	int32_t catchRopeSerial_;

	KnockBack knockBack_;

	int32_t startTimer_;

	Vector2 ropeDir_;

	int32_t addFeverCount_;

	int32_t groupSerial_;
	bool isLeader_;
	bool isRingCrush_;
	Vector3 crushPos_;
	TimerFloat crushEffectTimer_;

	bool isKnock_;
};

