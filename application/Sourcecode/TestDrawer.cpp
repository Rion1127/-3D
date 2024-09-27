#include "TestDrawer.h"
#include "AssimpObject3D.h"
#include "mInput.h"
#include <memory>

#include "ModelManager.h"

void TestDrawer::Init()
{
	//ステータスとLoadManagerで読み込んだassimpモデルの名前をペアにして入力
	objects_.objList_.insert(std::make_pair(TestState::ZERO, ModelManager::GetInstance()->GetAssimpModel("Human_run")));
	objects_.objList_.insert(std::make_pair(TestState::ONE, ModelManager::GetInstance()->GetAssimpModel("Human_jump")));
	//最初にどのアニメーションからスタートするか決めておく
	objects_.ChangeState(TestState::ZERO);
}

void TestDrawer::Update()
{
	
	if (Key::TriggerKey(DIK_O))
	{
		//ChangeState()でアニメーション切り替え（毎フレーム呼ぶと多分バグる）
		if (objects_.GetCurrentStateName() == TestState::ZERO) objects_.ChangeState(TestState::ONE);
		else if(objects_.GetCurrentStateName() == TestState::ONE) objects_.ChangeState(TestState::ZERO);
	}
	//ChangeState()の後に呼ぶこと。じゃないと1フレームだけ消えてちらつく
	objects_.currentObj_->SetScale(Vector3(0.1f, 0.1f, 0.1f));
	objects_.currentObj_->Update();
}

void TestDrawer::Draw()
{
	objects_.currentObj_->Draw();
}
