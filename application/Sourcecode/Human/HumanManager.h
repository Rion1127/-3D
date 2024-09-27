#pragma once
#include "BaseHuman.h"
#include "ColliderManager.h"
#include "Object3d.h"
#include "RescueCount.h"
#include "Timer.h"

class HumanManager
{
	//シングルトン
private:
	HumanManager() = default;
	~HumanManager() = default;
public:
	//コピーコンストラクタ無効
	HumanManager(const HumanManager& obj) = delete;
	//代入演算子を無効
	HumanManager& operator=(const HumanManager& obj) = delete;

	static HumanManager* GetInstance();

public:
	void Initialize(ColliderManager* colManagerPtr, RescueCount* rescueCountPtr,Timer* gameTimer);
	void Update();
	void Draw();

	void PopNormal(Vector2 pos,HumanState popState = HumanState::SPAWN);
	void PopDebu(Vector2 pos, HumanState popState = HumanState::SPAWN);
	void PopGroup(Vector2 pos,int32_t num, HumanState popState = HumanState::SPAWN);
	HumanState GetStateKnock();

	void AllDisengage();

	void ResetHumanList() { humanList_.clear(); }
	void SetPopData(std::vector<PopData> data) { popData_ = data; }

	Vector3 GetLeaderPos(int32_t num);
	int32_t GetLeaderTimer(int32_t num);

	Vector3 GetGroupPos(int32_t num);
	void SetSearchGroup(int32_t num,bool is);

	void SetIsKnockGroup(int32_t num);
	
	void SetIsFirstImpact() {
		isFirstImpact_ = true;
	}
	bool GetIsFirstImpact() {
		return isFirstImpact_;
	}

	Vector3 GetSpawnPoint(int num) {
		return spawnPointObj_[num]->GetPos();
	}

	void SetIsCatchSerial(int32_t serial);

	int32_t GetRandom07();
	Vector2 GetRandomVec2();

	std::vector<HumanType> GetHelpHumanType() { return helpHumanType_; }
private:
	void HumanPop();
private:
	std::vector<std::unique_ptr<BaseHuman>> humanList_;
	ColliderManager* colManagerPtr_;

	std::unique_ptr<Object3d> spawnPointObj_[3];
	RescueCount* rescueCountPtr_;

	int32_t groupCount_;
	Timer* gameTimerPtr_;

	std::vector<PopData> popData_;
	std::vector<HumanType> helpHumanType_;

	Timer feverPopHumanTimer_;

	bool isFirstImpact_;
};

