#include "Rescue_Humans.h"

Rescue_Humans* Rescue_Humans::GetInstance()
{
	static Rescue_Humans instance;
	return &instance;
}

void Rescue_Humans::Init()
{
	normal_human_objects_.clear();
	debu_human_objects_.clear();
	feverpoint_.clear();
	point_.clear();

	for (int i = 0; i < 10; i++) {
		WaitPoint point;
		point.pos = Vector3(-120 + (float)(8 * i), 20, 80);
		point_.emplace_back(point);
	}

	for (int i = 0; i < 10; i++) {
		WaitPoint point;
		point.pos = Vector3(50 + (float)(8 * i), 20, 80);
		point_.emplace_back(point);
	}

	rescueCloud_none_fever_y = 70;
	rescueCloud_fever_y = -100;

	rescueCloudLeft_.SetPos(Vector2(230, 70));
	rescueCloudRight_.SetPos(Vector2(1050, 70));
	rescueCloudLeft_.SetTexture(TextureManager::GetInstance()->GetTexture("Cloud_1"));
	rescueCloudRight_.SetTexture(TextureManager::GetInstance()->GetTexture("Cloud_2"));

	bigCloud_fever_y = 0;
	bigCloud_none_fever_y = -200;

	bigCloud_.SetPos(Vector2(600, bigCloud_none_fever_y));
	bigCloud_.SetTexture(TextureManager::GetInstance()->GetTexture("Big_cloud"));

	rescueHumanstate_ = RescueHumanState::NONE_FIVER;

	//for (int i = 0; i < 10; i++) {
	//	state_ = HumanState::SPAWN;
	//	std::unique_ptr<HumanDrawer> drawer = std::make_unique<HumanDrawer>();
	//	drawer->SetActiveShadow(false);
	//	drawer->Init(&state_);
	//	drawer->SetAnimationTimer(120);
	//	drawer->SetRepeatAnimation(true);
	//	drawer->SetScale(Vector3(1.2f, 1.2f, 1.2f));
	//	//パワーーーーーー
	//	drawer->Update();
	//	state_ = HumanState::KNOCKBACK;
	//	drawer->Update();
	//	drawer->SetDirVec(Vector2(0, -1));

	//	WaitPoint point;
	//	point.pos = Vector3(Vector3(-90.f + (i * 20.f), 20.f, 71));
	//	feverpoint_.emplace_back(point);

	//	drawer->SetPos(feverpoint_[i].pos);
	//	normal_human_objects_.emplace_back(std::move(drawer));
	//}
	for (int i = 0; i < 10; i++) {
		WaitPoint point;
		point.pos = Vector3(Vector3(-90.f + (i * 20.f), 20.f, 71));
		feverpoint_.emplace_back(point);
	}
}

void Rescue_Humans::Update()
{
	if (isFever_) {
		if (rescueHumanstate_ == RescueHumanState::NONE_FIVER) {
			rescueHumanstate_ = RescueHumanState::FIVER_FLOAT;
		}
	}

	if (rescueHumanstate_ == RescueHumanState::FIVER_FLOAT) {
		auto posLeft = rescueCloudLeft_.GetPos();
		auto posright = rescueCloudRight_.GetPos();
		if (posLeft.y > rescueCloud_fever_y) {
			posLeft.y -= 20;
			posright.y -= 20;
		}
		else {
			posLeft.y = rescueCloud_fever_y;
			posright.y = rescueCloud_fever_y;
		}
		//雲が上に移動する
		rescueCloudLeft_.SetPos(posLeft);
		rescueCloudRight_.SetPos(posright);
	}
	else if (rescueHumanstate_ == RescueHumanState::FIVER_HELP) {
		//フィーバーが終わったら
		if (isFever_ == false) {
			auto posLeft = rescueCloudLeft_.GetPos();
			auto posright = rescueCloudRight_.GetPos();
			if (posLeft.y < rescueCloud_none_fever_y) {
				posLeft.y += 20;
				posright.y += 20;
			}
			else {
				posLeft.y = rescueCloud_none_fever_y;
				posright.y = rescueCloud_none_fever_y;
			}
			//雲が上に移動する
			rescueCloudLeft_.SetPos(posLeft);
			rescueCloudRight_.SetPos(posright);

			//でかい雲
			auto bigCloud = bigCloud_.GetPos();
			if (bigCloud.y > bigCloud_none_fever_y) {
				bigCloud.y -= 20;
			}
			else {
				bigCloud.y = bigCloud_none_fever_y;
			}
			//雲が上に移動する
			bigCloud_.SetPos(bigCloud);
		}
		else if (isFever_) {
			auto bigCloud = bigCloud_.GetPos();
			if (bigCloud.y < bigCloud_fever_y) {
				bigCloud.y += 20;
			}
			else {
				bigCloud.y = bigCloud_fever_y;
			}
			//雲が上に移動する
			bigCloud_.SetPos(bigCloud);
		}
		else {
			
		}
	}
	else if (rescueHumanstate_ == RescueHumanState::FIVER_FALL) {
		auto bigCloud = bigCloud_.GetPos();
		if (bigCloud.y < bigCloud_fever_y) {
			bigCloud.y += 20;
		}
		else {
			bigCloud.y = bigCloud_fever_y;
		}
		//雲が上に移動する
		bigCloud_.SetPos(bigCloud);
	}
	else if (rescueHumanstate_ == RescueHumanState::FIVER_HELP) {
		
	}


	for (auto& obj : normal_human_objects_) {
		//上から落ちてきて手をふる
		if (rescueHumanstate_ == RescueHumanState::NONE_FIVER) {
			if (obj->GetPos().y > 0) {
				Vector3 pos = obj->GetPos();
				pos.y -= 1.f;
				obj->SetPos(pos);
			}
			else {
				obj->SetRepeatAnimation(true);
				state_ = HumanState::IDLE;
			}
			obj->SetRepeatAnimation(true);
			state_ = HumanState::IDLE;
		}
		//フィーバーで上に飛ぶ
		else if (rescueHumanstate_ == RescueHumanState::FIVER_FLOAT) {
			Vector3 pos = obj->GetPos();
			pos.y += 1.f;
			obj->SetPos(pos);

			if (pos.y >= 20) {
				rescueHumanstate_ = RescueHumanState::FIVER_FALL;
				PopHelpHuman();
				break;
			}
		}
		//でかい雲のうえに落ちる
		else if (rescueHumanstate_ == RescueHumanState::FIVER_FALL) {
			if (obj->GetPos().y > 0) {
				Vector3 pos = obj->GetPos();
				pos.y -= 1.f;
				obj->SetPos(pos);
			}
			else {
				obj->SetRepeatAnimation(true);
				state_ = HumanState::FEVER;
				rescueHumanstate_ = RescueHumanState::FIVER_HELP;
			}
		}
		//フィーバー中の一緒に助ける演出
		else if (rescueHumanstate_ == RescueHumanState::FIVER_HELP) {
			//フィーバーが終わったら
			if (isFever_ == false) {
				Vector3 pos = obj->GetPos();
				pos.y += 5.f;
				obj->SetPos(pos);
				if (pos.y >= 100) {
					rescueHumanstate_ = RescueHumanState::NONE_FIVER;
					normal_human_objects_.clear();
					debu_human_objects_.clear();

					for (int32_t i = 0; i < feverpoint_.size(); i++) {
						feverpoint_[i].isActive = true;
					}
					for (int32_t i = 0; i < point_.size(); i++) {
						point_[i].isActive = true;
					}
					break;
				}
			}
		}
		obj->Update();
	}
	//おでぶ
	for (auto& obj : debu_human_objects_) {
		//上から落ちてきて手をふる
		if (rescueHumanstate_ == RescueHumanState::NONE_FIVER) {
			if (obj->GetPos().y > 0) {
				Vector3 pos = obj->GetPos();
				pos.y -= 1.f;
				obj->SetPos(pos);
			}
			else {
				obj->SetRepeatAnimation(true);
				state_ = HumanState::IDLE;
			}
			obj->SetRepeatAnimation(true);
			state_ = HumanState::IDLE;
		}
		//フィーバーで上に飛ぶ
		else if (rescueHumanstate_ == RescueHumanState::FIVER_FLOAT) {
			Vector3 pos = obj->GetPos();
			pos.y += 1.f;
			obj->SetPos(pos);
			if (pos.y >= 20) {
				rescueHumanstate_ = RescueHumanState::FIVER_FALL;
				PopHelpHuman();
				break;
			}
		}
		//でかい雲のうえに落ちる
		else if (rescueHumanstate_ == RescueHumanState::FIVER_FALL) {
			if (obj->GetPos().y > 0) {
				Vector3 pos = obj->GetPos();
				pos.y -= 1.f;
				obj->SetPos(pos);
			}
			else {
				obj->SetRepeatAnimation(true);
				state_ = HumanState::FEVER;
				rescueHumanstate_ = RescueHumanState::FIVER_HELP;
			}
		}
		//フィーバー中の一緒に助ける演出
		else if (rescueHumanstate_ == RescueHumanState::FIVER_HELP) {
			if (isFever_ == false) {
				Vector3 pos = obj->GetPos();
				pos.y += 5.f;
				obj->SetPos(pos);
				if (pos.y >= 100) {
					rescueHumanstate_ = RescueHumanState::NONE_FIVER;
					normal_human_objects_.clear();
					debu_human_objects_.clear();
					break;
				}
			}
		}
		obj->Update();
	}

	rescueCloudLeft_.Update();
	rescueCloudRight_.Update();
	bigCloud_.Update();
}

void Rescue_Humans::Draw()
{
	for (auto& obj : normal_human_objects_) {
		obj->Draw();
	}
	for (auto& obj : debu_human_objects_) {
		obj->Draw();
	}
}

void Rescue_Humans::DrawSprite()
{
	rescueCloudLeft_.Draw();
	rescueCloudRight_.Draw();
	bigCloud_.Draw();
}

void Rescue_Humans::DrawFrontSprite()
{

}

void Rescue_Humans::PopHuman(HumanType type)
{
	if (isFever_) return;
	Vector3 popPos;
	for (int32_t i = 0; i < point_.size(); i++) {
		if (point_[i].isActive) {
			popPos = point_[i].pos;
			point_[i].isActive = false;
			break;
		}
	}

	if (type == HumanType::NORMAL) {
		state_ = HumanState::SPAWN;
		std::unique_ptr<HumanDrawer> drawer = std::make_unique<HumanDrawer>();
		drawer->Init(&state_);
		drawer->SetAnimationTimer(120);
		drawer->SetRepeatAnimation(true);
		//パワーーーーーー
		drawer->Update();
		state_ = HumanState::KNOCKBACK;
		drawer->Update();
		drawer->SetDirVec(Vector2(0, -1));
		drawer->SetPos(popPos);
		normal_human_objects_.emplace_back(std::move(drawer));
	}
	else if (type == HumanType::DEBU) {
		state_ = HumanState::SPAWN;
		std::unique_ptr<DebuHumanDrawer> drawer = std::make_unique<DebuHumanDrawer>();
		drawer->Init(&state_);
		drawer->SetAnimationTimer(120);
		drawer->SetRepeatAnimation(true);
		//パワーーーーーー
		drawer->Update();
		state_ = HumanState::KNOCKBACK;
		drawer->Update();
		drawer->SetDirVec(Vector2(0, -1));
		drawer->SetPos(popPos);
		debu_human_objects_.emplace_back(std::move(drawer));
	}
}

void Rescue_Humans::PopHelpHuman()
{
	if (feverpoint_[0].isActive == false)return;
	normal_human_objects_.clear();
	debu_human_objects_.clear();

	Vector3 popPos;
	for (int32_t i = 0; i < feverpoint_.size(); i++) {
		if (feverpoint_[i].isActive) {
			popPos = feverpoint_[i].pos;
			feverpoint_[i].isActive = false;
		}

		if (0 == (i % 2)) {
			state_ = HumanState::SPAWN;
			std::unique_ptr<HumanDrawer> drawer = std::make_unique<HumanDrawer>();
			drawer->Init(&state_);
			int32_t time;
			if (i == 0)time = 20;
			else time = 55;
			drawer->SetAnimationTimer(time);
			drawer->SetRepeatAnimation(true);
			drawer->SetScale(Vector3(1.2f, 1.2f, 1.2f));
			//パワーーーーーー
			drawer->Update();
			state_ = HumanState::KNOCKBACK;
			drawer->Update();
			drawer->SetDirVec(Vector2(0, -1));
			drawer->SetPos(popPos);
			normal_human_objects_.emplace_back(std::move(drawer));
		}
		else if (1 == (i % 2)) {
			state_ = HumanState::SPAWN;
			std::unique_ptr<DebuHumanDrawer> drawer = std::make_unique<DebuHumanDrawer>();
			drawer->Init(&state_);
			int32_t time;
			if (i == feverpoint_.size() - 1)time = 20;
			else time = 55;
			drawer->SetAnimationTimer(time);
			drawer->SetRepeatAnimation(true);
			drawer->SetScale(Vector3(1.2f, 1.2f, 1.2f));
			//パワーーーーーー
			drawer->Update();
			state_ = HumanState::KNOCKBACK;
			drawer->Update();
			drawer->SetDirVec(Vector2(0, -1));
			drawer->SetPos(popPos);
			debu_human_objects_.emplace_back(std::move(drawer));
		}
	}
}
