#include "BaseHuman.h"
#include "PlayerManager.h"
#include "RRandom.h"
#include <random>
#include "ModelManager.h"
#include "HumanManager.h"
#include "Tutorial.h"
#include "ParticleManager.h"
#include "Easing.h"

BaseHuman::~BaseHuman()
{
	collider_.Finalize();
	PlayerManager::GetInstance()->UnRegisterCatchHuman(this);
}

void BaseHuman::Initialize(
	Vector2 pos,
	Vector2 vec, const std::string& colName,
	ColliderManager* colManagerPtr,
	HumanState popState,
	int32_t groupNum,
	bool isLeader)
{
	isDead_ = false;
	isAddScore_ = true;
	pos_.x = pos.x;
	pos_.z = pos.y;

	pos_.y = 0;

	knockBack_.Init();

	scale_ = { radius_,5.0f,radius_ };
	searchRadius_ = radius_;

	auto callback = std::bind(&ColliderObject::CallBack, this);
	// コライダー|初期化関数実行
	collider_.Initialize(colName, callback, colManagerPtr);

	isCatch_ = false;

	//ハンマーのパワーを当たり判定に追加
	collider_.Data_Remove("isCatch");
	collider_.Data_Add("isCatch", isCatch_);

	state_ = popState;
	/*drawer_.SetAnimationTimer(20);
	drawer_.SetRepeatAnimation(true);
	drawer_.Init(&state_);*/
	if (groupNum != -1 && !isLeader) {
		startTimer_ = HumanManager::GetInstance()->GetLeaderTimer(groupNum);
	}
	else {
		startTimer_ = RRandom::Rand(50, 120);
	}

	moveVec_ = vec;
	moveVec_ = moveVec_.normalize();

	Vector2 temp2D = { PlayerManager::GetInstance()->GetRopeEndPos().x - pos_.x,PlayerManager::GetInstance()->GetRopeEndPos().z - pos_.z };
	ropeDir_ = temp2D.normalize();

	ling_ = std::move(std::make_unique<Object3d>());
	ling_->SetModel(ModelManager::GetInstance()->GetModel("Ring"));
	groupSerial_ = groupNum;
	isLeader_ = isLeader;
	catchRopeSerial_ = -1;

	isRingCrush_ = false;
	crushEffectTimer_.SetLimitTime(20.f);
}

void BaseHuman::Update()
{
	Vector2 temp2D = { PlayerManager::GetInstance()->GetNearRope(pos_).x - pos_.x,PlayerManager::GetInstance()->GetNearRope(pos_).z - pos_.z };
	//移動制限
	if (state_ != HumanState::SPAWN &&
		state_ != HumanState::CATCH) {
		Vector3 limit = pos_;
		limit.y = 40;
		if (limit.length() > MOVE_RADIUSE) {
			pos_ = {
				limit.normalize().x * MOVE_RADIUSE,
				pos_.y,
				limit.normalize().z * MOVE_RADIUSE,
			};
		}
	}
	collider_.sphere_.center = pos_;

	if (PlayerManager::GetInstance()->GetPos().y <= pos_.y) {
		//ロープを引き上げている最中じゃないと人を引き上げない
		if (PlayerManager::GetInstance()->GetSwitchingTimer() != 0 || PlayerManager::GetInstance()->GetNearRopeSerial(pos_) != 0) {
			Rescue();
		}
	}
	else if (pos_.y < 0) {
		pos_.y = 0;
	}

	//オブジェクトの更新
	ropeDir_ = temp2D.normalize();
	//if(state_ != HumanState::SPAWN)drawer_.SetDirVec();
	//drawer_.SetPos(pos_);
	//drawer_.Update();

	if (isLeader_) {
		Vector3 temp = HumanManager::GetInstance()->GetGroupPos(groupSerial_);
		ling_->SetPos({ temp.x,temp.y + 7.f,temp.z });
		ling_->SetScale({ 10,10,10 });
		ling_->Update();

		crushPos_ = ling_->GetPos();
	}
	else if (isRingCrush_)
	{
		crushEffectTimer_.AddTime();
		if (crushEffectTimer_.GetIsEnd())
		{
			isRingCrush_ = false;
		}

		float scale = Easing::Back::easeIn(10.f,15.f,crushEffectTimer_.GetTimeRate());
		float height = Easing::Back::easeOut(10.f,0.f,crushEffectTimer_.GetTimeRate());
		ling_->SetPos({ crushPos_.x,crushPos_.y,crushPos_.z });
		ling_->SetScale({ scale,height,scale });
		ling_->Update();
	}
}

void BaseHuman::Draw()
{
	//オブジェクトの描画
	if (isLeader_ || isRingCrush_) {
		PipelineManager::PreDraw("Object3D", TRIANGLELIST);
		ling_->Draw();
		PipelineManager::PreDraw("assimp", TRIANGLELIST);
	}
}

void BaseHuman::Rescue()
{
	if (catchRopeSerial_ == 0) {
		PlayerManager::GetInstance()->AddFeverCount(addFeverCount_);
	}
	SetDead();
}

void BaseHuman::SetDead()
{
	isDead_ = true;
}

void BaseHuman::CatchRope()
{
	isCatch_ = true;
	catchNum_ = PlayerManager::GetInstance()->GetCatchCount();
	if (groupSerial_ != -1) {
		HumanManager::GetInstance()->SetIsCatchSerial(groupSerial_);
	}
}
