#include "DebuHuman.h"
#include "ModelManager.h"

void DebuHuman::Initialize(
	Vector2 pos,
	Vector2 vec,
	const std::string& colName,
	ColliderManager* colManagerPtr,
	HumanState popState,
	int32_t group,
	bool isLeader)
{
	//3Dオブジェクト
	obj_ = std::move(std::make_unique<Object3d>());
	//モデル読み込み
	obj_->SetModel(ModelManager::GetInstance()->GetModel("Player"));

	radius_ = 5.f;
	speed_ = 0.3f;
	weight_ = 2;
	addFeverCount_ = 3;

	BaseHuman::Initialize(pos, vec, colName, colManagerPtr, popState,group,isLeader);
	drawer_.SetAnimationTimer(20);
	drawer_.SetRepeatAnimation(true);
	drawer_.Init(GetHumanState());
	collider_.sphere_.radius = 5.f;

	InitStateMachine();
	drawer_.SetDirVec(ropeDir_);
	humanType_ = HumanType::DEBU;
}

void DebuHuman::Update()
{
	//プレイヤーの状態更新
	StateMachineUpdate();
	BaseHuman::Update();

	if (*GetHumanState() != HumanState::SPAWN &&
		*GetHumanState() != HumanState::KNOCKBACK &&
		*GetHumanState() != HumanState::WAKEUP)
	{
		drawer_.SetDirVec(ropeDir_);
	}
	drawer_.SetPos(pos_);
	drawer_.Update();
}

void DebuHuman::Draw()
{
	//オブジェクトの描画
	drawer_.Draw();
}

void DebuHuman::CallBack()
{
	if (collider_.IsTrigger_Name("swing_rope"))
	{
		//すでにつかまっていたら飛ばないようにする
		if (isCatch_) return;
		SphereCollider* ropeCol = static_cast<SphereCollider*>(collider_.Extract_Collider("swing_rope"));
		//nullチェック
		if (ropeCol == nullptr) return;
		//振り回しているフラグがfalseだったら吹き飛ばさない
		bool isSwing = ropeCol->Data_Get<bool>("isSwing");
		if (isSwing == false) return;
		Vector3 knockVec = collider_.sphere_.center - ropeCol->sphere_.center;
		knockVec = knockVec.normalize();
		knockVec.y = 1.0f;
		knockBack_.SetKnockVeclocity(knockVec * 0.7f);
		//ノックバックでロープを追いかけるのをやめる
		isSearch_ = false;
		GoToState(HumanState::KNOCKBACK);
	}

	if (collider_.IsDetect_Name("NormalHuman"))
	{
		SphereCollider* ropeCol = static_cast<SphereCollider*>(collider_.Extract_Collider("NormalHuman"));
		if (ropeCol == nullptr) return;
		//ロープにつかまっていたら無効にする
		bool isCatch = ropeCol->Data_Get<bool>("isCatch");
		if (isCatch)return;
		Vector3 EtoEVec = ropeCol->sphere_.center - collider_.sphere_.center;
		//ベクトルの長さを取得
		float length = EtoEVec.length();
		EtoEVec = EtoEVec.normalize();
		//Y成分を無効にして押し出す
		EtoEVec.y = 0;
		//二つの当たり判定の半径の長さを足す
		float backLength = ropeCol->sphere_.radius + collider_.sphere_.radius;
		//ベクトルの長さを引いてめり込んでいる長さ分だけ押し戻す()
		backLength -= length;

		Vector3 pushBackVec = (EtoEVec * backLength);

		pos_ += -pushBackVec;
	}

	if (collider_.IsDetect_Name("DebuHuman"))
	{
		SphereCollider* ropeCol = static_cast<SphereCollider*>(collider_.Extract_Collider("DebuHuman"));
		if (ropeCol == nullptr) return;
		//ロープにつかまっていたら無効にする
		bool isCatch = ropeCol->Data_Get<bool>("isCatch");
		if (isCatch)return;
		Vector3 EtoEVec = ropeCol->sphere_.center - collider_.sphere_.center;
		//ベクトルの長さを取得
		float length = EtoEVec.length();
		EtoEVec = EtoEVec.normalize();
		//Y成分を無効にして押し出す
		EtoEVec.y = 0;
		//二つの当たり判定の半径の長さを足す
		float backLength = ropeCol->sphere_.radius + collider_.sphere_.radius;
		//ベクトルの長さを引いてめり込んでいる長さ分だけ押し戻す()
		backLength -= length;

		Vector3 pushBackVec = (EtoEVec * backLength);

		pos_ += -pushBackVec;
	}
}

void DebuHuman::InitStateMachine()
{
	// 状態を追加していく。
	AddState(std::make_shared<DebuHuman_Idle>(this));
	AddState(std::make_shared<DebuHuman_Chase>(this));
	AddState(std::make_shared<DebuHuman_Catch>(this));
	AddState(std::make_shared<DebuHuman_Spawn>(this));
	AddState(std::make_shared<DebuHuman_KnockBack>(this));
	AddState(std::make_shared<DebuHuman_WakeUp>(this));


	GoToState(*GetHumanState());
}
