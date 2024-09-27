#include "HumanManager.h"
#include "NormalHuman.h"
#include "DebuHuman.h"
#include "mInput.h"
#include <random>
#include "ModelManager.h"
#include "PlayerManager.h"
#include "Rescue_Humans.h"
#include "RRandom.h"


HumanManager* HumanManager::GetInstance()
{
	static HumanManager instance;
	return &instance;
}

void HumanManager::Initialize(ColliderManager* colManagerPtr, RescueCount* rescueCountPtr, Timer* gameTimer)
{
	gameTimerPtr_ = gameTimer;
	humanList_.clear();
	helpHumanType_.clear();
	colManagerPtr_ = colManagerPtr;
  
	for (int i = 0; i < 3; i++) {
		//3Dオブジェクト
		spawnPointObj_[i] = std::move(std::make_unique<Object3d>());
		//モデル読み込み
		spawnPointObj_[i]->SetModel(ModelManager::GetInstance()->GetModel("Player"));
	}

	spawnPointObj_[0]->SetPos({ 58,1,-58 });
	spawnPointObj_[1]->SetPos({ 0,1,-80 });
	spawnPointObj_[2]->SetPos({ -58,1,-58 });

	for (int i = 0; i < 3; i++) {
		spawnPointObj_[i]->Update();
	}
	rescueCountPtr_ = rescueCountPtr;

	groupCount_ = 0;

	feverPopHumanTimer_.SetLimitTime(60);

	isFirstImpact_ = false;
}

void HumanManager::Update()
{
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetIsDead()) {
			bool isAddCsore = humanList_[i]->GetIsAddScore();
			auto humanType = humanList_[i]->GetHumanType();
			humanList_.erase(humanList_.begin() + i);
			if (rescueCountPtr_ == nullptr)continue;
			//得点を追加するフラグがtrueじゃないと加算しない
			if (isAddCsore)
			{
				rescueCountPtr_->AddCount();
				SoundManager::Play("CountSE");
				Rescue_Humans::GetInstance()->PopHuman(humanType);
				helpHumanType_.emplace_back(humanType);
			}
			
		}
	}

	//仮生成
	if (Key::TriggerKey(DIK_1)) {
		int32_t temp = GetRandom07();
		PopNormal({ spawnPointObj_[temp]->GetPos().x, spawnPointObj_[temp]->GetPos().z });
	}
	if (Key::TriggerKey(DIK_2)) {
		int32_t temp = GetRandom07();
		PopDebu({ spawnPointObj_[temp]->GetPos().x, spawnPointObj_[temp]->GetPos().z });
	}
	if (Key::TriggerKey(DIK_3)) {
		int32_t temp = GetRandom07();
		PopGroup({ spawnPointObj_[temp]->GetPos().x, spawnPointObj_[temp]->GetPos().z },5);
	}
	//人の出現
	HumanPop();

	for (int i = 0; i < humanList_.size(); i++) {
		humanList_[i]->Update();
		if (humanList_[i]->GetIsCatch()) {
			//ロープにつかまっている状態のフラグを建てる
			PlayerManager::GetInstance()->SetIsCatch(true);
			if (humanList_[i]->GetCatchRopeSerial() == 0) {
				PlayerManager::GetInstance()->AddWeight(humanList_[i]->GetWeight());
			}
		}
#ifdef DEBUG
		ImGui::Begin("human");
		ImGui::Text("pos = %f,%f,%f", humanList_[i]->GetPos().x, humanList_[i]->GetPos().y, humanList_[i]->GetPos().z);
		ImGui::End();
#endif // DEBUG
	}

	for (int i = 0; i < 3; i++) {
		spawnPointObj_[i]->Update();
	}
}

void HumanManager::Draw()
{
	for (int i = 0; i < humanList_.size(); i++) {
		humanList_[i]->Draw();
	}
	for (int i = 0; i < 3;i++) {
		spawnPointObj_[i]->Draw();
	}
}

void HumanManager::PopNormal(Vector2 pos, HumanState popState)
{
	std::unique_ptr<BaseHuman> newHuman = std::make_unique<NormalHuman>();
	newHuman->Initialize(pos + GetRandomVec2(),pos * -1, "NormalHuman", colManagerPtr_, popState);
	humanList_.push_back(std::move(newHuman));
}

void HumanManager::PopDebu(Vector2 pos, HumanState popState)
{
	std::unique_ptr<BaseHuman> newHuman = std::make_unique<DebuHuman>();
	newHuman->Initialize(pos + GetRandomVec2(), pos * -1, "DebuHuman", colManagerPtr_, popState);
	humanList_.push_back(std::move(newHuman));
}

void HumanManager::PopGroup(Vector2 pos, int32_t num,HumanState popState)
{
	//リーダー
	{
		std::unique_ptr<BaseHuman> newHuman = std::make_unique<NormalHuman>();
		newHuman->Initialize(pos, pos * -1, "NormalHuman", colManagerPtr_, popState, groupCount_, 1);
		humanList_.push_back(std::move(newHuman));
	}
	//取り巻き
	for (int i = 0; i < num;i++) {
		std::unique_ptr<BaseHuman> newHuman = std::make_unique<NormalHuman>();
		newHuman->Initialize(pos + GetRandomVec2() * 0.01f, pos * -1, "NormalHuman", colManagerPtr_, popState, groupCount_, 0);
		humanList_.push_back(std::move(newHuman));
	}

	groupCount_++;
}

HumanState HumanManager::GetStateKnock()
{
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetState() == HumanState::KNOCKBACK) {
			return humanList_[i]->GetState();
		}
	}
	
	return HumanState();
}

void HumanManager::AllDisengage()
{
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetCatchRopeSerial() == 0) {
			humanList_[i]->SetIsCatch(false);
		}
	}
}

Vector3 HumanManager::GetLeaderPos(int32_t num)
{
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetGroupNum() == num && humanList_[i]->GetIsLeader()) {
			return humanList_[i]->GetPos();
		}
	}

	return {0,0,0};
}

int32_t HumanManager::GetLeaderTimer(int32_t num)
{
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetGroupNum() == num && humanList_[i]->GetIsLeader()) {
			return humanList_[i]->GetStartTimer();
		}
	}

	return 120;
}

Vector3 HumanManager::GetGroupPos(int32_t num)
{
	Vector3 pos = {};
	int32_t count = 0;

	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetGroupNum() == num) {
			pos += humanList_[i]->GetPos();
			count++;
		}
	}

	return pos / (float)count;
}

void HumanManager::SetSearchGroup(int32_t num ,bool is)
{
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetGroupNum() == num) {
			humanList_[i]->SetIsSearch(is);
		}
	}
}

void HumanManager::SetIsKnockGroup(int32_t num)
{
	if (num == -1) {
		return;
	}
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetGroupNum() == num) {
			humanList_[i]->SetIsKnock();
		}
	}
}

void HumanManager::SetIsCatchSerial(int32_t serial)
{
	if (serial == -1) {
		return;
	}
	for (int i = 0; i < humanList_.size(); i++) {
		if (humanList_[i]->GetGroupNum() == serial) {
			humanList_[i]->SetIsCatch(true);
		}
	}
}

int32_t HumanManager::GetRandom07()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> x(0.f, 2.99999f);

	return (int32_t)x(engine);
}

Vector2 HumanManager::GetRandomVec2()
{
	//ランダム
	std::random_device seed_gen;
	std::mt19937_64 engine(seed_gen());

	std::uniform_real_distribution<float> x(-12.f, 12.f);
	std::uniform_real_distribution<float> y(-12.f, 12.f);

	return { x(engine),y(engine) };
}

void HumanManager::HumanPop()
{
	if (gameTimerPtr_ != nullptr) {
		for (auto& data : popData_) {
			//すでにポップ済みの場合はスキップする
			if (data.isPop == false)continue;
			int32_t time = gameTimerPtr_->GetTimer();
			if (time == data.popTime) {
				for (int32_t i = 0; i < data.popPos1.popNum_normal; i++) {
					PopNormal({ spawnPointObj_[2]->GetPos().x, spawnPointObj_[2]->GetPos().z });
				}
				for (int32_t i = 0; i < data.popPos1.popNum_debu; i++) {
					PopDebu({ spawnPointObj_[2]->GetPos().x, spawnPointObj_[2]->GetPos().z });
				}
				for (int32_t i = 0; i < data.popPos1.popNum_group; i++) {
					PopGroup({ spawnPointObj_[2]->GetPos().x, spawnPointObj_[2]->GetPos().z }, data.popPos1.popNum_group_humans);
				}

				for (int32_t i = 0; i < data.popPos2.popNum_normal; i++) {
					PopNormal({ spawnPointObj_[1]->GetPos().x, spawnPointObj_[1]->GetPos().z });
				}
				for (int32_t i = 0; i < data.popPos2.popNum_debu; i++) {
					PopDebu({ spawnPointObj_[1]->GetPos().x, spawnPointObj_[1]->GetPos().z });
				}
				for (int32_t i = 0; i < data.popPos2.popNum_group; i++) {
					PopGroup({ spawnPointObj_[1]->GetPos().x, spawnPointObj_[1]->GetPos().z }, data.popPos2.popNum_group_humans);
				}

				for (int32_t i = 0; i < data.popPos3.popNum_normal; i++) {
					PopNormal({ spawnPointObj_[0]->GetPos().x, spawnPointObj_[0]->GetPos().z });
				}
				for (int32_t i = 0; i < data.popPos3.popNum_debu; i++) {
					PopDebu({ spawnPointObj_[0]->GetPos().x, spawnPointObj_[0]->GetPos().z });
				}
				for (int32_t i = 0; i < data.popPos3.popNum_group; i++) {
					PopGroup({ spawnPointObj_[0]->GetPos().x, spawnPointObj_[0]->GetPos().z }, data.popPos3.popNum_group_humans);
				}
				data.isPop = false;
			}
			
		}
	}
	//フィーバー中の湧き
	if (PlayerManager::GetInstance()->GetIsFever()) {
		feverPopHumanTimer_.AddTime();
		if (feverPopHumanTimer_.GetIsEnd()) {
			feverPopHumanTimer_.Reset();
			//人の数が70を超えないようにする
			if (humanList_.size() > 70) return;
			for (int32_t i = 0; i < 3; i++) {
				int32_t lottery = RRandom::Rand(0, 100);
				if (lottery <= 75) {
					PopNormal({ spawnPointObj_[i]->GetPos().x, spawnPointObj_[i]->GetPos().z });
				}
				else {
					PopDebu({ spawnPointObj_[i]->GetPos().x, spawnPointObj_[i]->GetPos().z });
				}
			}
		}
	}
	else {
		feverPopHumanTimer_.Reset();
	}
}
