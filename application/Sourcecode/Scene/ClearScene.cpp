#include "ClearScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "TitleScene.h"
#include "Easing.h"
#include "Timer.h"
#include "mSound.h"

#include "Ranking.h"

#include <fstream>
#include <iostream>

/**
 * @file ClearScene.cpp
 * @brief メインのゲームシーンクラス
 */

ClearScene::~ClearScene()
{
	Ranking::GetInstance()->Reset();
}

void ClearScene::Init()
{
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	sound_ = SoundManager::GetInstance();

	SoundManager::AllStop();

	//SoundManager::Play("ClearSE");

	resultSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Result"));
	Vector2 resultPos = {
		WinAPI::GetWindowSize().x / 7.f,
		WinAPI::GetWindowSize().y / 6.f,
	};
	resultSprite_.SetPos(resultPos);
	resultSprite_.SetScale(Vector2(0.8f, 0.8f));
	resultSprite_.SetRot(Radian(-10));

	retryTextSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Retry"));
	retryPos = {
		WinAPI::GetWindowSize().x * 0.2f,
		WinAPI::GetWindowSize().y * 0.87f,
	};
	retryTextSprite_.SetPos(retryPos);
	retryTextSprite_.SetScale(Vector2(0.5f, 0.5f));

	titleTextSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("ToTitle"));
	titlePos = {
		WinAPI::GetWindowSize().x * 0.8f,
		WinAPI::GetWindowSize().y * 0.87f,
	};
	titleTextSprite_.SetPos(titlePos);
	titleTextSprite_.SetScale(Vector2(0.5f, 0.5f));

	//背景
	backGround_.SetTexture(TextureManager::GetInstance()->GetTexture("Background"));
	Vector2 backgroundPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	backGround_.SetPos(backgroundPos);

	//UI初期化
	uiSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("AButton"));
	Vector2 uiPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.15f,
	};
	uiSprite_.SetPos(uiPos);
	uiSprite_.SetSplitNum(Vector2(2, 1));
	Vector2 texSize = {
		uiSprite_.GetTexture().size_.x / 2.f,
		uiSprite_.GetTexture().size_.y,
	};
	uiSprite_.SetTex_Size(texSize);

	uiEasingTimer_.SetLimitTime(50.f);
	uiEasingTimer_.Reset();

	uiFloatTimer_.SetLimitTime(40);

	Ranking::GetInstance()->Init();

	LoadRanking();
	RankSpriteInit();

	sound_->AllBGMStop();
	sound_->Play("ResultBGM", true);

	isSEPray = false;
	isRetry = true;
}

void ClearScene::Update()
{
#ifdef _DEBUG

#endif // _DEBUG
	lightManager_->Update();

	Ranking::GetInstance()->Update();

	Rank();

	//コントローラー
	if (Controller::GetActive()) {
		isSceneChange_ = Controller::GetTriggerButtons(PAD::INPUT_A);
		if (Controller::GetTriggerButtons(PAD::INPUT_A)) {
			isSceneChange_ = true;
			SoundManager::Play("DecisionSE");
		}

		//リトライするかタイトル戻るか選択
		if (Controller::GetTriggerButtons(PAD::INPUT_RIGHT) || Controller::GetLStick().x > 0.f)
		{
			isRetry = false;
			SoundManager::Play("SelectSE");
		}
		else if (Controller::GetTriggerButtons(PAD::INPUT_LEFT) || Controller::GetLStick().x < 0.f)
		{
			isRetry = true;
			SoundManager::Play("SelectSE");
		}
	}
	//キーボード
	else {
		if ((Key::TriggerKey(DIK_RETURN) || Key::TriggerKey(DIK_SPACE))) {
			isSceneChange_ = true;
			SoundManager::Play("DecisionSE");
		}

		//リトライするかタイトル戻るか選択
		if (Key::TriggerKey(DIK_D))
		{
			isRetry = false;
			SoundManager::Play("SelectSE");
		}
		else if (Key::TriggerKey(DIK_A))
		{
			isRetry = true;
			SoundManager::Play("SelectSE");
		}
	}

	if (isSceneChange_) {
		if (Ranking::GetInstance()->GetEasingTimer().GetIsEnd() == false) {
			isSceneChange_ = false;
			Ranking::GetInstance()->EasingSkip();
		}
		else if (isRetry) {
			SceneManager::SetChangeStart(SceneName::Game);
		}
		else
		{
			SceneManager::SetChangeStart(SceneName::Title);
		}
	}

	static bool isBackEnd = false;
	if (isBackEnd == false)
	{
		uiEasingTimer_.AddTime(1.f);
		if (uiEasingTimer_.GetIsEnd())
		{
			uiEasingTimer_.SetIsEnd(false);
			isBackEnd = true;
		}
	}
	else
	{
		uiEasingTimer_.AddTime(-1.f);
		if (uiEasingTimer_.GetTimer() <= 0.f)
		{
			//uiEasingTimer_.SetIsEnd(false);
			isBackEnd = false;
		}
	}


	float scale = Easing::Back::easeOut(0.5f,0.7f, uiEasingTimer_.GetTimeRate());
	if (isRetry)
	{
		retryTextSprite_.SetScale(Vector2(scale, scale));
		titleTextSprite_.SetScale(Vector2(0.5f, 0.5f));
	}
	else
	{
		retryTextSprite_.SetScale(Vector2(0.5f, 0.5f));
		titleTextSprite_.SetScale(Vector2(scale, scale));
	}

	Vector2 texLeftTop = {
		uiSprite_.GetTexture().size_.x / 2.f,
		0,
	};
	texLeftTop.x *= uiAnimationIndex_;
	uiSprite_.SetTex_LeftTop(texLeftTop);
	uiSprite_.SetScale(Vector2(0.2f, 0.4f));

	//UIのアニメーション更新
	uiFloatTimer_.AddTime();
	if (uiFloatTimer_.GetIsEnd()) {
		uiFloatTimer_.Reset();
		uiAnimationIndex_++;
		if (uiAnimationIndex_ > 1) uiAnimationIndex_ = 0;

	}

	if (Ranking::GetInstance()->GetEasingTimer().GetIsEnd() && isSEPray == false)
	{
		SoundManager::Play("ResultEndSE");
		isSEPray = true;
	}

	uiSprite_.Update();
	rankItemSprite_.Update();
	rankTextSprite_.Update();

	resultSprite_.Update();
	retryTextSprite_.Update();
	titleTextSprite_.Update();
	backGround_.Update();

#ifdef _DEBUG
	if (Framework::isImguiDisplay_) {
		ImGui::Begin("Timer");
		//プルダウンメニューでイージングタイプを選択できるようにする
		ImGui::Text("残り時間:%f", uiEasingTimer_.GetTimer());

		ImGui::End();
	}
#endif // _DEBUG
}

void ClearScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	backGround_.Draw();
	resultSprite_.Draw();
	retryTextSprite_.Draw();
	titleTextSprite_.Draw();
	uiSprite_.Draw();
	rankItemSprite_.Draw();
	rankTextSprite_.Draw();
	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);

	///////////////////
	//フロントスプライト//
	//////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	Ranking::GetInstance()->Draw();

	PipelineManager::PreDraw("Particle", POINTLIST);
}

void ClearScene::DrawRenderTexture()
{

}

void ClearScene::DrawPostEffect()
{
	PipelineManager::PreDraw("Particle", POINTLIST);
}

void ClearScene::LoadRanking()
{
	std::vector<int32_t> result_(3);
	//読み込み
	std::ifstream file("application/Resources/Ranking/Ranking.txt");  // 読み込むファイルのパスを指定
	std::string line;

	while (std::getline(file, line)) {  // 1行ずつ読み込む
		std::cout << line << std::endl;

		std::stringstream line_stream(line);

		// 半角スペース区切りで行の先頭文字列を取得
		std::string key;
		getline(line_stream, key, ' ');
		if (key == "1") {
			line_stream >> result_[0];
		}
		else if (key == "2") {
			line_stream >> result_[1];
		}
		else if (key == "3") {
			line_stream >> result_[2];
		}
	}
	//降順でソート
	std::sort(result_.rbegin(), result_.rend());

	Ranking::GetInstance()->SetRanking(result_);
}

void ClearScene::RankSpriteInit()
{
	rankItemSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Rank"));
	rankItemSprite_.SetPos(Vector2(450.f, 350.f));
	rankItemSprite_.SetScale(Vector2(0.5f, 0.5f));

	rankTextSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Rank"));
	rankTextSprite_.SetPos(Vector2(900.f, 350.f));
	rankTextSprite_.SetScale(Vector2(0.5f, 0.5f));
}

void ClearScene::Rank()
{
	//画像の切り替え
	int32_t score = Ranking::GetInstance()->GetEasingNewScore();
	std::string textureName;
	if (score >= Rank::S) textureName = "S_Rank";
	else if (score >= Rank::A) textureName = "A_Rank";
	else if (score >= Rank::B) textureName = "B_Rank";
	else if (score >= Rank::C) textureName = "C_Rank";

	rankTextSprite_.SetTexture(TextureManager::GetInstance()->GetTexture(textureName));
}
