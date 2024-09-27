#pragma once
#include "Object3d.h"
#include "SphereCollider.h"
#include "ColliderManager.h"
#include "FileOutPut.h"
#include "Human/BaseHuman.h"
#include "GameStatus.h"
#include "Timer.h"
#include "ParticleTrail.h"

#include "ParticleManager.h"

class Rope
{
public:
	void Initialize(Vector3 pos, ColliderManager* colManagerPtr, bool autoMode = false,int32_t serial = 0);
	void Update(Vector3 pos = {-999.f,0.f,0.f});
	void Draw();
	void DrawLight();
	void Finalize();
	Rope() = default;
	~Rope();
	//当たり判定の処理
	void CallBack();

	Vector3 GetEndPos(int32_t num = ROPE_LENGTH - 1) {return pos_[num]; }
	bool GetIsCatch() {return isCatch_;}
	void SetIsCatch(bool is) {isCatch_ = is;}
	void AddWeight(int32_t num) {weight_ += num;}
	bool GetIsCut() {return isCut_;}
	bool GetIsUse() {return isUse_;}
	float GetCatchRadius() {return catchRadius_;}
	void AddCatchCount() {catchCount_++;}
	int32_t GetCatchCount() {return catchCount_;}
	int32_t GetSwitchingTimer() {return switchingTimer_;}
	bool GetIsSwing() {	return isSwing_;}
	bool GetIsDead() {return isDead_;}
	int32_t GetSerial() {return serial_;}
	int32_t GetRopeCount() {return catchCount_;}

	std::vector<BaseHuman*> GetCatchHuman() { return ropeCatchHumanList_; }

	void RegisterCatchHuman(BaseHuman* human) {
		ropeCatchHumanList_.emplace_back(human);
	};
	void UnRegisterCatchHuman(BaseHuman* human) { // std::removeを使って、対象の要素を取り除く
		ropeCatchHumanList_.erase(
			std::remove(ropeCatchHumanList_.begin(), ropeCatchHumanList_.end(), human),
			ropeCatchHumanList_.end()
		);
	};

	//ファイルに書き込む
	void Save();
	void Load();
private:

	static const int32_t ROPE_LENGTH = 10;
	float distance = 4.f;
	std::unique_ptr<Object3d> ropeObj_[ROPE_LENGTH];
	Vector3 pos_[ROPE_LENGTH];
	Vector3 vec_[ROPE_LENGTH];
	Vector3 oldPos_;

	int32_t switchingTime_ = 80;
	float switchingSpeed_ = 0.6f;
	int32_t switchingTimer_;
	bool isUse_;//糸を出しているかどうか
	float ropeY_;

	float swingWeakRange_ = 5.f;
	float swingStrongRange_ = 10.f;
	bool isSwing_;//振り回し判定
	bool prevIsSwing_;//誰かが掴んでいるか 前フームの情報を取得
	SwingPower swingPower_;
	SwingPower prevSwingPower_;//振り回し判定 前フームの情報を取得
	int32_t swingTimer_ = 0;
	int32_t swingTime_ = 20;

	float catchRadius_;
	std::unique_ptr<Object3d> catchObj_;
	bool isCatch_;//誰かが掴んでいるか
	int32_t catchCount_;
	Vector3 catchPos_;
	float catchAngle_;

	const int32_t MAX_WEIGHT = 6;//ロープがきれる重さ
	int32_t weight_;//ロープの重さ
	bool isCut_;
	const int32_t CUT_TIME = 100;
	int32_t cutTimer;

	//振り回している時の当たり判定
	SphereCollider swingCollider_;

	float nonCatchNormal_ = 0.3f;
	float nonCatchAdd_ = 0.002f;
	float nonCatchSubNormal_ = 0.93f;
	float nonCatchSubAdd_ = 0.02f;
	float catchNormal_ = 0.06f;
	float catchSub_ = 0.98f;

	bool isAuto_;
	bool isDead_ = false;

	FileOutPut output_;

	int32_t serial_;
	std::vector<BaseHuman*> ropeCatchHumanList_;	//ロープにつかまっている人のポインタを格納
	std::unique_ptr<Object3d> reliefLight_;
	Timer lightTimer_;
	Timer firstLightTimer_;	//引き上げるときの明かりのタイマー
	//パーティクル
	std::shared_ptr<ContinuousEmitter> fireEmitter_;
	std::unique_ptr<ParticleTrail> trail_;
private:
	//演出系変数
	const int32_t MaxSwingSEInterval = 60;
	Timer swingSETimer_;
};

