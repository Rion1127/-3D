#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "ClearScene.h"
#include <filesystem>
#include "mSound.h"
#include "Framework.h"
#include "Ranking.h"
#include "Rescue_Humans.h"
//プレイヤーマネージャー
#include "Field/FieldManager.h"
#include "PlayerManager.h"
#include "Human/HumanManager.h"
#include "Camera/CameraManager.h"
#include "Tutorial.h"

#include <fstream>
#include <iostream>
#include <algorithm>

/**
 * @file GameScene.cpp
 * @brief メインのゲームシーンクラス
 */

GameScene::~GameScene(void)
{
	//ランキング更新 
	if (timer_.GetIsEnd()) RankingUpdate();
	HumanManager::GetInstance()->ResetHumanList();
	PlayerManager::GetInstance()->Finalize();
	ParticleManager::GetInstance()->AllClear();
}

void GameScene::Init()
{
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLight(lightManager_->GetLightGroup());
	//////////////////////////////////
	//3Dオブジェクト
	object3d_ = std::move(std::make_unique<Object3d>());
	//モデル読み込み
	//object3d_->SetModel(Model::CreateOBJ_uniptr("field", true, false));
	//////////////////////////////////

	background_.SetTexture(TextureManager::GetInstance()->GetTexture("Background"));
	Vector2 backgroundPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	background_.SetPos(backgroundPos);

	readyText_.SetTexture(TextureManager::GetInstance()->GetTexture("Ready"));
	Vector2 readyTextPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	readyText_.SetPos(readyTextPos);
	readyText_.SetColor(Color(255, 255, 255, 0));

	startText_.SetTexture(TextureManager::GetInstance()->GetTexture("StartCall"));
	Vector2 startTextPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	startText_.SetPos(startTextPos);
	startText_.SetColor(Color(255, 255, 255, 0));

	isReadySEPlay = false;
	isStartSEPlay = false;

	isTimerActiveDebug_ = true;
	isTimerActive_ = false;

	timerGauge_.sprite_.SetTexture(TextureManager::GetInstance()->GetTexture("TimerGauge"));
	//ここパワーだからカメラアングル変わったら調整しなおしです；；
	//ホントは疑似3Dオブジェクトみたいなビュー変換にしたい
	Vector2 timerGaugePos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.65f,
	};
	timerGauge_.sprite_.SetPos(timerGaugePos);
	timerGauge_.sprite_.SetScale({ 1.55f,1.15f });

	feverGauge_.sprite_.SetTexture(TextureManager::GetInstance()->GetTexture("FeverGauge"));
	//ここパワーだからカメラアングル変わったら調整しなおしです；；
	//ホントは疑似3Dオブジェクトみたいなビュー変換にしたい
	Vector2 feverGaugePos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	feverGauge_.sprite_.SetPos(feverGaugePos);
	feverGauge_.sprite_.SetScale({ 2.f,2.f });

	FieldManager::GetInstance()->Initialize();
	PlayerManager::GetInstance()->Initialize(&colliderManager_);
	HumanManager::GetInstance()->Initialize(&colliderManager_, &rescueCount_, &timer_);
	CameraManager::GetInstance()->Initialize();
	Rescue_Humans::GetInstance()->Init();

	rescueCount_.Reset();
	rescueCount_.SetPos(Vector2(1050, 650));
	rescueCount_.SetTextColor(Color(0,0,0, 255));
	rescueCount_.SetNumberLocalPos(Vector2(40, 0));

	scoreOutPut_.SetfileDir("application/Resources/Ranking/");

	isCanOutPut_ = true;

	feverLight_.Init();
	feverLight_.SetIsFever(true);
	SoundManager::AllBGMStop();
	SoundManager::Play("GameplayBGM", true);
	auto popData = editor_.Load();
	HumanManager::GetInstance()->SetPopData(popData);
	timer_.SetLimitTime(gameLimitTime);
	countdownTimer_.SetLimitTime(countTime_);
	startEaseTimer_.SetLimitTime(startEaseTime_);

	fireEmitter_ = std::make_shared<ContinuousEmitter>();
	TimerFloat timer;
	timer.SetLimitTime(5);
	timer.SetTime(timer.GetLimitTimer());
	fireEmitter_->popCoolTime_ = timer;
	fireEmitter_->pos = { 0,65.f,0 };
	//ParticleManager::GetInstance()->AddParticle("light", fireEmitter_);

	ringCrushEmitter_ = std::make_shared<OneceEmitter>();
	ringCrushEmitter_->pos = { 0,20,0 };
	ParticleManager::GetInstance()->AddParticle("ringCrush", ringCrushEmitter_);

	/*ascensionEmitter_ = std::make_shared<OneceEmitter>();
	ascensionEmitter_->pos = {0,65.f,0};
	ParticleManager::GetInstance()->AddParticle("light", ascensionEmitter_);*/
}

void GameScene::Update()
{
#ifdef _DEBUG
	if (Key::PushKey(DIK_LCONTROL)) {
		isDebug_ = true;
	}
	//カメラ更新
	debugCamera_.Update();
	if (isDebug_) {
		Camera::scurrent_ = debugCamera_.GetCamera();
	}
	else {
		Camera::scurrent_ = CameraManager::GetInstance()->GetCamera();
	}
	Camera::scurrent_->Update(CameraMode::LookAT);
#endif // _DEBUG
	if (timer_.GetIsEnd()) {
		SceneManager::SetChangeStart(SceneName::GameClear);
	}

	lightManager_->Update();

	colliderManager_.Update();

	if (countdownTimer_.GetTimeRate() > 0.15f && isReadySEPlay == false)
	{
		SoundManager::Play("ResultSE");
		isReadySEPlay = true;
	}
	countdownTimer_.AddTime();

	if (countdownTimer_.GetTimeRate() > 0.9f &&
		isTimerActive_ == false && isStartSEPlay == false)
	{
		isTimerActive_ = true;
		SoundManager::Play("DecisionSE");
		isStartSEPlay = true;
	}

	if (countdownTimer_.GetTimeRate() > 0.15f)
	{
		readyText_.SetScale(Vector2(3.5f - countdownTimer_.GetTimeRate() * 2.f, 3.5f - countdownTimer_.GetTimeRate() * 2.f));
		readyText_.SetColor(Color(255.f, 255.f, 255.f, Easing::Back::easeOut(255.f, 0.f, countdownTimer_.GetTimeRate())));
	}

	if (isTimerActive_) {
		startEaseTimer_.AddTime();
	}

	if (isTimerActive_ && startEaseTimer_.GetIsEnd() == false)
	{
		startText_.SetScale(Vector2(3.5f - startEaseTimer_.GetTimeRate() * 2.f, 3.5f - startEaseTimer_.GetTimeRate() * 2.f));
		startText_.SetRot(-0.25f + startEaseTimer_.GetTimeRate() * 0.5f);
		startText_.SetColor(Color(255.f, 255.f, 255.f, Easing::Back::easeOut(200.f, 0.f, startEaseTimer_.GetTimeRate())));
	}

	//タイマーゲージはタイマーの進み具合とリンク
	timerGauge_.rate_ = 1.0f - timer_.GetTimeRate();
	timerGauge_.Update();

	Tutorial::GetInstance()->Update();

	if (PlayerManager::GetInstance()->GetIsFever())
	{
		feverGauge_.rate_ = PlayerManager::GetInstance()->GetFeverTimerRate();
		feverGauge_.Update();
	}
	else
	{
		feverGauge_.rate_ = PlayerManager::GetInstance()->GetFeverCountRate();
		feverGauge_.Update();

		//ゲームの制限時間
		if (isTimerActive_ && isTimerActiveDebug_ && Tutorial::GetInstance()->GetIsTutorial()) {
			timer_.AddTime();
		}
	}
	feverLight_.SetIsFever(PlayerManager::GetInstance()->GetIsFever());
	feverLight_.Update();
	Rescue_Humans::GetInstance()->SetIsFever(PlayerManager::GetInstance()->GetIsFever());
	//オブジェクトの更新
	//object3d_->Update();
	FieldManager::GetInstance()->Update();
	PlayerManager::GetInstance()->Update(&colliderManager_);
	HumanManager::GetInstance()->Update();
	CameraManager::GetInstance()->Update();
	Rescue_Humans::GetInstance()->Update();
	ParticleManager::GetInstance()->Update();
	rescueCount_.Update();
	if (!isDebug_) {
		Camera::scurrent_ = CameraManager::GetInstance()->GetCamera();
	}
	Camera::scurrent_->Update(CameraMode::LookAT);

	background_.Update();
	readyText_.Update();
	startText_.Update();
#ifdef _DEBUG
	if (Framework::isImguiDisplay_) {
		ImGui::Begin("Timer");
		//プルダウンメニューでイージングタイプを選択できるようにする
		ImGui::Text("残り時間:%d", timer_.GetLimitTimer() - timer_.GetTimer());
		ImGui::Checkbox("タイマー有効化", &isTimerActiveDebug_);
		bool isTimerReset = false;
		ImGui::Checkbox("タイマーリセット", &isTimerReset);
		if (isTimerReset)
		{
			timer_.Reset();
			isTimerReset = false;
		}

		ImGui::End();
	}
#endif // _DEBUG
}

void GameScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	background_.Draw();
	Rescue_Humans::GetInstance()->DrawSprite();
	//sprite_1.Draw();
	//sprite_2.Draw();

	/////////////////
	//3Dオブジェクト//
	/////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	//オブジェクトの描画
	//object3d_->Draw();
	FieldManager::GetInstance()->Draw();

	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	//フィーバー中の雲
	Rescue_Humans::GetInstance()->DrawSprite();
	//タイマーゲージはフィールドの一層上に描画
	PipelineManager::PreDraw("CircleGauge", TRIANGLELIST);
	timerGauge_.Draw();
	feverGauge_.Draw();

	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	//プレイヤー描画
	PlayerManager::GetInstance()->Draw();

	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	//人描画
	HumanManager::GetInstance()->Draw();
	Rescue_Humans::GetInstance()->Draw();
	ParticleManager::GetInstance()->Draw();
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	feverLight_.Draw();
	PlayerManager::GetInstance()->DrawLight();
	///////////////////
	//フロントスプライト//
	//////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	rescueCount_.Draw();
	readyText_.Draw();
	startText_.Draw();
	Tutorial::GetInstance()->Draw();
	PlayerManager::GetInstance()->SpriteDraw();
	
}

void GameScene::DrawRenderTexture()
{
}

void GameScene::DrawPostEffect()
{
	PipelineManager::PreDraw("Particle", POINTLIST);
}

void GameScene::RankingUpdate()
{
	if (isCanOutPut_ == false)return;
	std::vector<int32_t> result_(4);
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
	result_[3] = rescueCount_.GetRescueNum();
	//降順でソート
	std::sort(result_.rbegin(), result_.rend());
	//ファイルに出力
	scoreOutPut_.Begin("Ranking");
	for (int32_t i = 0; i < 3; i++) {
		std::string itemname;
		std::ostringstream oss;
		oss << (i + 1);
		itemname = oss.str();

		scoreOutPut_.Output(itemname.c_str(), result_[i]);
	}
	scoreOutPut_.End();

	Ranking::GetInstance()->SetRanking(result_);
	Ranking::GetInstance()->SetNewScore(rescueCount_.GetRescueNum());

	isCanOutPut_ = false;
}