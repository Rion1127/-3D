#include "AssimpDrawer.h"
#include "AssimpObject3D.h"
#include "mInput.h"
#include <memory>

#include "ModelManager.h"

void AssimpDrawer::Init(TestState* state)
{
	state_ = state;
	//ステータスとLoadManagerで読み込んだassimpモデルの名前をペアにして入力
	objects_.objList_.insert(std::make_pair(TestState::IDLE, ModelManager::GetInstance()->GetAssimpModel("Human_run")));
	objects_.objList_.insert(std::make_pair(TestState::WALK, ModelManager::GetInstance()->GetAssimpModel("Human_run")));
	objects_.objList_.insert(std::make_pair(TestState::RUN, ModelManager::GetInstance()->GetAssimpModel("Human_run")));
	//最初にどのアニメーションからスタートするか決めておく
	objects_.ChangeState(*state_);
	objects_.currentObj_->SetAnimationTime(animationTimer_);
	objects_.currentObj_->SetRepeatAnimation(isRepeatAnimation_);
}

void AssimpDrawer::Update()
{
	if (state_ != nullptr) {
		//ステートが切り替わっていた場合モデルを変更する
		if (prevState_ != *state_) {
			objects_.ChangeState(*state_);
			objects_.currentObj_->SetAnimationTime(animationTimer_);
			objects_.currentObj_->SetRepeatAnimation(isRepeatAnimation_);
		}
		prevState_ = *state_;
	}

	//ChangeState()の後に呼ぶこと。じゃないと1フレームだけ消えてちらつく
	objects_.currentObj_->SetScale(Vector3(0.1f, 0.1f, 0.1f) * scale_);
	objects_.currentObj_->SetPos(pos_);
	//向きを回転
	float rotY = Vec2Angle(dirVec_);
	objects_.currentObj_->SetRot(Vector3(0,Radian(rotY), 0));
	objects_.currentObj_->Update();

	dirty_ = false;
}

void AssimpDrawer::Draw()
{
	objects_.currentObj_->Draw();
}
