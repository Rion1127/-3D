#pragma once
#include "Player.h"
#include "Rope.h"
#include "ColliderManager.h"
#include "Sprite.h"
#include "mSound.h"

class PlayerManager
{
	//シングルトン
private:
	PlayerManager() = default;
	~PlayerManager() = default;
public:
	//コピーコンストラクタ無効
	PlayerManager(const PlayerManager& obj) = delete;
	//代入演算子を無効
	PlayerManager& operator=(const PlayerManager& obj) = delete;

	static PlayerManager* GetInstance();

public:
	void Initialize(ColliderManager* colManagerPtr);
	void Update(ColliderManager* colManagerPtr);
	void Draw();
	void SpriteDraw();
	void DrawLight();
	void Finalize();

	//プレイヤーの位置(ロープの一番上)
	Vector3 GetPos() {
		return player_.GetOffsetPos();
	}

	//ロープの一番下
	Vector3 GetRopeEndPos(int num = 9) {
		return rope_.GetEndPos(num);
	}

	//一番近いロープを探索
	Vector3 GetNearRope(Vector3 pos);
	int32_t GetNearRopeSerial(Vector3 pos);
	Vector3 GetSerialRopePos(int32_t serial, int32_t num = 9);

	//誰かがロープに捕まっているか
	bool GetIsCatch() {
		return rope_.GetIsCatch();
	}
	void SetIsCatch(bool is,int32_t num = 0);

	//重さ関係
	void AddWeight(int32_t num) {
 		rope_.AddWeight(num);
	}

	bool GetIsCut() {
		return rope_.GetIsCut();
	}

	bool GetIsUse() {
		return rope_.GetIsUse();
	}

	float GetCatchRadius() {
		return rope_.GetCatchRadius();
	}

	void AddCatchCount() {
		rope_.AddCatchCount();
	}
	int32_t GetCatchCount() {
		return rope_.GetCatchCount();
	}
	int32_t GetSwitchingTimer() {
		return rope_.GetSwitchingTimer();
	}

	bool isGetSwing() {
		return rope_.GetIsSwing();
	}

	bool GetIsFever() {
		return isFever_;
	}
	int32_t GetFeverCount() {
		return feverCount_;
	}
	float GetFeverCountRate() {
		return float(feverCount_) / float(MAX_FEVER_COUNT);
	}
	void AddFeverCount(int32_t count) {
		if(!isFever_) feverCount_ += count;
	}
	float GetFeverTimer() {
		return feverTimer_;
	}
	float GetFeverTimerRate() {
		return feverTimer_ / FEVER_TIME;
	}

	int32_t GetRopeCount() {
		return rope_.GetRopeCount();
	}

	void RegisterCatchHuman(BaseHuman* human) { rope_.RegisterCatchHuman(human); }
	void UnRegisterCatchHuman(BaseHuman* human) { rope_.UnRegisterCatchHuman(human);}
	Rope* GetRopePtr() { return &rope_; }
private:
	Player player_;
	Rope rope_;

	int32_t serial_;

	std::vector<std::unique_ptr<Rope>> feverRopeList_;
	bool isFever_;

	const int32_t MAX_FEVER_COUNT = 20;
	int32_t feverCount_ = 0;

	const float FEVER_TIME = 500.f;
	float feverTimer_;

	Sprite feverSp_;
	float spRot_;

	SoundManager* sound_ = nullptr;
};

