#include "FatHumanDrawer.h"
#include "AssimpObject3D.h"
#include "mInput.h"
#include <memory>

#include "ModelManager.h"

void DebuHumanDrawer::Init(HumanState* state)
{
	state_ = state;
	//ステータスとLoadManagerで読み込んだassimpモデルの名前をペアにして入力
	objects_.objList_.insert(std::make_pair(HumanState::SPAWN, ModelManager::GetInstance()->GetAssimpModel("Fatman_run")));
	objects_.objList_.insert(std::make_pair(HumanState::IDLE, ModelManager::GetInstance()->GetAssimpModel("Fatman_wait")));
	objects_.objList_.insert(std::make_pair(HumanState::WALK, ModelManager::GetInstance()->GetAssimpModel("Fatman_run")));
	objects_.objList_.insert(std::make_pair(HumanState::CHASE, ModelManager::GetInstance()->GetAssimpModel("Fatman_run")));
	objects_.objList_.insert(std::make_pair(HumanState::CATCH, ModelManager::GetInstance()->GetAssimpModel("Fatman_jump")));
	objects_.objList_.insert(std::make_pair(HumanState::KNOCKBACK, ModelManager::GetInstance()->GetAssimpModel("Fatman_knockback")));
	objects_.objList_.insert(std::make_pair(HumanState::WAKEUP, ModelManager::GetInstance()->GetAssimpModel("Fatman_wakeup")));
	objects_.objList_.insert(std::make_pair(HumanState::FEVER, ModelManager::GetInstance()->GetAssimpModel("FatHuman_fever")));
	//最初にどのアニメーションからスタートするか決めておく
	objects_.ChangeState(*state_);
	objects_.currentObj_->SetAnimationTime(animationTimer_);
	objects_.currentObj_->SetRepeatAnimation(isRepeatAnimation_);
}

void DebuHumanDrawer::Update()
{

	if (Key::TriggerKey(DIK_O)) {
		//ChangeState()でアニメーション切り替え（毎フレーム呼ぶと多分バグる）
		if (objects_.GetCurrentStateName() == HumanState::IDLE) objects_.ChangeState(HumanState::WALK);
		else if (objects_.GetCurrentStateName() == HumanState::WALK) objects_.ChangeState(HumanState::CATCH);
		else if (objects_.GetCurrentStateName() == HumanState::CATCH) objects_.ChangeState(HumanState::IDLE);
	}

	if (state_ != nullptr) {
		if (prevState_ != *state_) {
			objects_.ChangeState(*state_);
			objects_.currentObj_->SetAnimationTime(animationTimer_);
			objects_.currentObj_->SetRepeatAnimation(isRepeatAnimation_);
		}
		prevState_ = *state_;
	}

	//ChangeState()の後に呼ぶこと。じゃないと1フレームだけ消えてちらつく
	objects_.currentObj_->SetScale(Vector3(0.13f, 0.13f, 0.13f) * scale_);
	if (dirty_)pos_.y += 0.3f;
	objects_.currentObj_->SetPos(pos_);

	float rotY = Vec2Angle(dirVec_);

	objects_.currentObj_->SetRot(Vector3(0,Radian(rotY), 0));
	objects_.currentObj_->Update();

	dirty_ = false;
}

void DebuHumanDrawer::Draw()
{
	objects_.currentObj_->Draw();
}
