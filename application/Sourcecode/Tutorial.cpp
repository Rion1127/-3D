#include "Tutorial.h"
#include "SceneManager.h"
#include "mInput.h"

Tutorial* Tutorial::GetInstance()
{
	static Tutorial instance;
	return &instance;
}

void Tutorial::Update()
{
	Vector2 texLeftTop;
	if ((Key::GetKeyReleased(DIK_Z) || Controller::GetButtons(PAD::INPUT_B)) && !isTutorial_) {
		skipTimer_++;
		texLeftTop = { skipSp_.GetTexture().size_.x / 2.f,0};
	}
	else {
		skipTimer_ = 0;
		texLeftTop = { 0,0 };
	}

	if (skipTimer_ > SKIP_TIME) {
		tutorialNum_ = 6;
	}

	//チュートリアルをやるか
	if (tutorialNum_ == 0) {
		if (!isTutorial_) {
			//イニシャライズ処理
			tutorialNum_ = 0;

			tutorialSp_.SetTexture(TextureManager::GetInstance()->GetTexture("Tutorial1"));
			tutorialSp_.SetPos({ 650,600 });
			tutorialSp_.SetColor({ 255,255,255,230 });

			skipSp_.SetTexture(TextureManager::GetInstance()->GetTexture("BButton"));
			skipSp_.SetPos({ 150,50 });
			skipSp_.SetScale({ 0.5f,0.5f });
			skipSp_.SetColor({ 255,255,255,230 });
			skipSp_.SetSplitNum(Vector2(2, 1));
			Vector2 texSize = {
				skipSp_.GetTexture().size_.x / 2.f,
				skipSp_.GetTexture().size_.y,
			};
			skipSp_.SetTex_Size(texSize);

			//次の処理
			tutorialNum_++;
			HumanManager::GetInstance()->PopNormal({ HumanManager::GetInstance()->GetSpawnPoint(0).x,HumanManager::GetInstance()->GetSpawnPoint(0).z });
		}
	}
	else if (tutorialNum_ == 1) {
		//掴んだら進む
		if (PlayerManager::GetInstance()->GetRopeCount() >= 1) {
			tutorialSp_.SetTexture(TextureManager::GetInstance()->GetTexture("Tutorial2"));
			tutorialNum_++;
		}
	}
	else if (tutorialNum_ == 2) {
		//引きあげたら進む(一般人ポップ)
		if (PlayerManager::GetInstance()->GetRopeCount() == 0) {
			tutorialSp_.SetTexture(TextureManager::GetInstance()->GetTexture("Tutorial1"));
			tutorialNum_++;
			HumanManager::GetInstance()->PopNormal({ HumanManager::GetInstance()->GetSpawnPoint(0).x,HumanManager::GetInstance()->GetSpawnPoint(0).z });
			HumanManager::GetInstance()->PopNormal({ HumanManager::GetInstance()->GetSpawnPoint(2).x,HumanManager::GetInstance()->GetSpawnPoint(2).z });
		}
	}
	else if (tutorialNum_ == 3) {
		//掴んだら進む
		if (PlayerManager::GetInstance()->GetRopeCount() >= 1) {
			tutorialSp_.SetTexture(TextureManager::GetInstance()->GetTexture("Tutorial3"));
			tutorialNum_++;
		}
	}
	else if (tutorialNum_ == 4) {
		//ノックバックさせたら進む(一般人ポップ)
		if (HumanManager::GetInstance()->GetStateKnock()) {
			tutorialSp_.SetTexture(TextureManager::GetInstance()->GetTexture("Tutorial4"));
			tutorialNum_++;
			HumanManager::GetInstance()->PopNormal({ HumanManager::GetInstance()->GetSpawnPoint(2).x,HumanManager::GetInstance()->GetSpawnPoint(2).z });
		}
	}
	else if (tutorialNum_ == 5) {
		//デブインパクトさせたら進む
		if (HumanManager::GetInstance()->GetIsFirstImpact()) {
			tutorialNum_++;
		}
	}
	else if (tutorialNum_ == 6) {
		SceneManager::SetChangeStart(SceneName::Game);
		tutorialNum_++;
		isTutorial_ = true;
	}

	skipSp_.SetTex_LeftTop(texLeftTop);
	skipSp_.Update();
	tutorialSp_.Update();
}

void Tutorial::Draw()
{
	if (!isTutorial_ && tutorialNum_ != 0) {
		skipSp_.Draw();
		tutorialSp_.Draw();
	}
}
