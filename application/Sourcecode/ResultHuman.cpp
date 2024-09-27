#include "ResultHuman.h"
#include "Human/HumanManager.h"

void ResultHuman::Init()
{
	helpHumanType_ = HumanManager::GetInstance()->GetHelpHumanType();
}

void ResultHuman::Update()
{
	for (auto& obj : normal_human_objects_) {
		obj->Update();
	}
	for (auto& obj : debu_human_objects_) {
		obj->Update();
	}
}

void ResultHuman::Draw()
{
	for (auto& obj : normal_human_objects_) {
		obj->Draw();
	}
	for (auto& obj : debu_human_objects_) {
		obj->Draw();
	}
}

void ResultHuman::PopHuman()
{
	for (int32_t i = 0; i < helpHumanType_.size(); i++) {

		/*if (feverpoint_[i].isActive) {
			popPos = feverpoint_[i].pos;
			feverpoint_[i].isActive = false;
		}*/
		Vector3 popPos;
		if (helpHumanType_[i] == HumanType::NORMAL) {
			state_ = HumanState::SPAWN;
			std::unique_ptr<HumanDrawer> drawer = std::make_unique<HumanDrawer>();
			drawer->Init(&state_);
			int32_t time = 55;
			drawer->SetAnimationTimer(time);
			drawer->SetRepeatAnimation(true);
			drawer->SetScale(Vector3(1.2f, 1.2f, 1.2f));
			//パワーーーーーー
			drawer->Update();
			state_ = HumanState::FEVER;
			drawer->Update();
			drawer->SetDirVec(Vector2(0, -1));
			drawer->SetPos(popPos);
			normal_human_objects_.emplace_back(std::move(drawer));
		}
		else if (helpHumanType_[i] == HumanType::DEBU) {
			state_ = HumanState::SPAWN;
			std::unique_ptr<DebuHumanDrawer> drawer = std::make_unique<DebuHumanDrawer>();
			drawer->Init(&state_);
			int32_t time = 55;
			drawer->SetAnimationTimer(time);
			drawer->SetRepeatAnimation(true);
			drawer->SetScale(Vector3(1.2f, 1.2f, 1.2f));
			//パワーーーーーー
			drawer->Update();
			state_ = HumanState::FEVER;
			drawer->Update();
			drawer->SetDirVec(Vector2(0, -1));
			drawer->SetPos(popPos);
			debu_human_objects_.emplace_back(std::move(drawer));
		}

	}
}
