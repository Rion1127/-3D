#include "NormalHuman.h"
#include "PlayerManager.h"
#include "HumanManager.h"

void NormalHuman::Initialize(
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
	//obj_->SetModel(Model::CreateOBJ_uniptr("player", true, false));

	weight_ = 1;

	radius_ = 5.0f;
	speed_ = 0.3f;
	if (group != -1) {
		speed_ = 1.f;
	}
	addFeverCount_ = 1;
	BaseHuman::Initialize(pos,vec, colName, colManagerPtr, popState,group,isLeader);
	drawer_.SetAnimationTimer(20);
	drawer_.SetRepeatAnimation(true);
	drawer_.Init(GetHumanState());
	if (group != -1) {
		collider_.sphere_.radius = 2.f;
	}
	else {
		collider_.sphere_.radius = 5.f;
	}

	InitStateMachine();
	drawer_.SetDirVec(ropeDir_);
	humanType_ = HumanType::NORMAL;

	isKnock_ = false;
}

void NormalHuman::Update()
{
	//プレイヤーの状態更新
	StateMachineUpdate();
	BaseHuman::Update();

	if (isKnock_) {
		Knock();
	}

	if (*GetHumanState() != HumanState::SPAWN &&
		*GetHumanState() != HumanState::KNOCKBACK &&
		*GetHumanState() != HumanState::WAKEUP)
	{
		drawer_.SetDirVec(ropeDir_);
	}
	drawer_.SetPos(pos_);
	drawer_.Update();
}

void NormalHuman::Draw()
{
	//オブジェクトの描画
	drawer_.Draw();
	BaseHuman::Draw();
}

void NormalHuman::CallBack()
{
	if (collider_.IsTrigger_Name("swing_rope")) {
		//すでにつかまっていたら飛ばないようにする
		if (isCatch_) return;
		if (isDead_) return;
		SphereCollider* ropeCol = static_cast<SphereCollider*>(collider_.Extract_Collider("swing_rope"));
		//nullチェック
		if (ropeCol == nullptr) return;
		//振り回しているフラグがfalseだったら吹き飛ばさない
		bool isSwing = ropeCol->Data_Get<bool>("isSwing");
		if (isSwing == false) return;
		//振り回している力を取得
		SwingPower swingPower = ropeCol->Data_Get<SwingPower>("swingPower");
		if (swingPower == SwingPower::WEAK) {
			if (groupSerial_ != -1) {
				HumanManager::GetInstance()->SetIsKnockGroup(groupSerial_);
			}
			//吹き飛ばす処理（デブがロープにつかまってぶつかった場合）
			Vector3 knockVec = collider_.sphere_.center - ropeCol->sphere_.center;
			knockVec = knockVec.normalize();
			knockVec.y = 1.0f;
			knockBack_.SetKnockVeclocity(knockVec * 0.7f);
			//ノックバックでロープを追いかけるのをやめる
			isSearch_ = false;
			GoToState(HumanState::KNOCKBACK);
		}
		else if(swingPower == SwingPower::STRONG){
			//NormalHumanがつかまっているか検索する
			const auto& humanlist = PlayerManager::GetInstance()->GetRopePtr()->GetCatchHuman();
			bool isExistNormalHuman = false;	//NormalHumanがいる場合trueにする
			int32_t temp = -1;
			for (auto& human : humanlist) {
				const auto colName = human->GetColName();
				if (colName == "NormalHuman") {
					isExistNormalHuman = true;
					temp = human->GetGroupNum();
					break;	//一人でもNormalHumanがいたら検索終了
				}
			}
			if (isExistNormalHuman) {
				//デブにする（ロープに一人でもNormalHumanがつかまっていた場合）
				isDead_ = true;
				isAddScore_ = false;
				Vector2 pos = {
					collider_.sphere_.center.x,
					collider_.sphere_.center.z,
				};
				HumanManager::GetInstance()->PopDebu(pos, HumanState::KNOCKBACK);
				HumanManager::GetInstance()->SetIsKnockGroup(temp);
				SoundManager::Play("FatImpactSE", false, 0.7f);
				HumanManager::GetInstance()->SetIsFirstImpact();
			}
			else {
				//吹き飛ばす処理（デブがロープにつかまってぶつかった場合）
				Vector3 knockVec = collider_.sphere_.center - ropeCol->sphere_.center;
				knockVec = knockVec.normalize();
				knockVec.y = 1.0f;
				knockBack_.SetKnockVeclocity(knockVec * 0.7f);
				//ノックバックでロープを追いかけるのをやめる
				isSearch_ = false;
				GoToState(HumanState::KNOCKBACK);
				HumanManager::GetInstance()->SetIsKnockGroup(temp);
			}
		}
	}

	if (collider_.IsDetect_Name("NormalHuman")) {
		PushBack("NormalHuman");
	}

	if (collider_.IsDetect_Name("DebuHuman")) {
		PushBack("DebuHuman");
	}
}

void NormalHuman::Knock()
{
	//吹き飛ばす処理（デブがロープにつかまってぶつかった場合）
	Vector3 knockVec = collider_.sphere_.center - PlayerManager::GetInstance()->GetRopeEndPos();
	knockVec = knockVec.normalize();
	knockVec.y = 1.0f;
	knockBack_.SetKnockVeclocity(knockVec * 0.7f);
	//ノックバックでロープを追いかけるのをやめる
	isSearch_ = false;
	isKnock_ = false;
	GoToState(HumanState::KNOCKBACK);
}

void NormalHuman::PushBack(const std::string& colName)
{
	SphereCollider* ropeCol = static_cast<SphereCollider*>(collider_.Extract_Collider(colName));
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

void NormalHuman::InitStateMachine()
{
	// 状態を追加していく。
	AddState(std::make_shared<NormalHuman_Idle>(this));
	AddState(std::make_shared<NormalHuman_Chase>(this));
	AddState(std::make_shared<NormalHuman_Catch>(this));
	AddState(std::make_shared<NormalHuman_Spawn>(this));
	AddState(std::make_shared<Normalhuman_KnockBack>(this));
	AddState(std::make_shared<NormalHuman_WakeUp>(this));


	GoToState(*GetHumanState());
}
