#include "PlayerManager.h"
#include <random>
#include "imgui.h"
#include "mInput.h"

PlayerManager* PlayerManager::GetInstance()
{
	static PlayerManager instance;
	return &instance;
}

void PlayerManager::Initialize(ColliderManager* colManagerPtr)
{
	player_.Initialize();
	rope_.Initialize(player_.GetOffsetPos(), colManagerPtr);

	//0はメインの糸なので1からスタート
	serial_ = 1;

	feverCount_ = 0;
	feverTimer_ = 0;

	feverSp_.SetTexture(TextureManager::GetInstance()->GetTexture("Fever"));
	feverSp_.SetPos({650,350});
	feverSp_.SetColor({255,255,255,150});
	feverSp_.SetScale({1.5f,1.5f});

	sound_ = SoundManager::GetInstance();
	sound_->AllBGMStop();
}

void PlayerManager::Update(ColliderManager* colManagerPtr)
{
	for (int i = 0; i < feverRopeList_.size(); i++) {
		if (feverRopeList_[i]->GetIsDead()) {
			feverRopeList_[i]->Finalize();
			feverRopeList_.erase(feverRopeList_.begin() + i);
		}
	}

	if (isFever_ && (Key::TriggerKey(DIK_SPACE) || Controller::GetReleasButtons(PAD::INPUT_A))) {
		std::unique_ptr<Rope> newRope = std::make_unique<Rope>();
		newRope->Initialize(player_.GetOffsetPos(), colManagerPtr, true, serial_);
		serial_++;
		feverRopeList_.push_back(std::move(newRope));
	}
	player_.SetIsFever(isFever_);
	player_.Update();
	rope_.Update(player_.GetOffsetPos());
	for (int i = 0; i < feverRopeList_.size(); i++) {
		feverRopeList_[i]->Update();
	}

	if (feverCount_ >= MAX_FEVER_COUNT) {
		feverTimer_ = FEVER_TIME;
		feverCount_ = 0;
		sound_->Stop("GameplayBGM");
		sound_->Play("FeverBGM", true,0.1f);
	}
	
	if (feverTimer_ > 0) {
		--feverTimer_;
		isFever_ = true;
		
	}
	else {
		if (isFever_)
		{
			sound_->Play("GameplayBGM", true);
		}
		isFever_ = false;
		sound_->Stop("FeverBGM");
	}
#ifdef DEBUG
	//imgui
	ImGui::Begin("fever");
	ImGui::Checkbox("isFever", &isFever_);
	ImGui::SliderInt("feverCount", &feverCount_, 0, MAX_FEVER_COUNT);
	ImGui::SliderFloat("feverTimer", &feverTimer_, 0, 500);
	ImGui::End();
#endif // DEBUG

	spRot_ += 0.07f;
	feverSp_.SetRot(sin(spRot_) * 0.3f);
	feverSp_.Update();
}

void PlayerManager::Draw()
{
	player_.Draw();

	rope_.Draw();
	for (int i = 0; i < feverRopeList_.size(); i++) {
		feverRopeList_[i]->Draw();
	}
}

void PlayerManager::SpriteDraw()
{
	if (isFever_) {
		feverSp_.Draw();
	}
}

void PlayerManager::DrawLight()
{
	rope_.DrawLight();
}

void PlayerManager::Finalize()
{
	rope_.Finalize();
}

Vector3 PlayerManager::GetNearRope(Vector3 pos)
{
	Vector3 ans = rope_.GetEndPos();
	float len = (pos - rope_.GetEndPos()).length();

	for (int i = 0; i < feverRopeList_.size(); i++) {
		if ((pos - feverRopeList_[i]->GetEndPos()).length() < len) {
			len = (pos - feverRopeList_[i]->GetEndPos()).length();
			ans = feverRopeList_[i]->GetEndPos();
		}
	}

	return ans;
}

int32_t PlayerManager::GetNearRopeSerial(Vector3 pos)
{
	int32_t ans = 0;
	float len = (pos - rope_.GetEndPos()).length();

	for (int i = 0; i < feverRopeList_.size(); i++) {
		if ((pos - feverRopeList_[i]->GetEndPos()).length() < len) {
			len = (pos - feverRopeList_[i]->GetEndPos()).length();
			ans = feverRopeList_[i]->GetSerial();
		}
	}

	return ans;
}

Vector3 PlayerManager::GetSerialRopePos(int32_t serial,int32_t num)
{
	if (num != 0) {
		for (int i = 0; i < feverRopeList_.size(); i++) {
			if (feverRopeList_[i]->GetSerial() == serial) {
				return feverRopeList_[i]->GetEndPos(num);
			}
		}
	}

	return rope_.GetEndPos(num);
}

void PlayerManager::SetIsCatch(bool is, int32_t num)
{
	if (num == 0) {
		rope_.SetIsCatch(is);
	}
	else {
		for (int i = 0; i < feverRopeList_.size(); i++) {
			if (feverRopeList_[i]->GetSerial() == num) {
				feverRopeList_[i]->SetIsCatch(is);
			}
		}
	}
}
