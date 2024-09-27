#include "TitleScene.h"
#include "SceneManager.h"
#include "Field/FieldManager.h"
#include "Camera/CameraManager.h"
#include "Tutorial.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	sound_ = SoundManager::GetInstance();
	//タイトルロゴ
	title_.SetTexture(TextureManager::GetInstance()->GetTexture("Logo_only_shadow"));
	Vector2 titlePos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	title_.SetPos(titlePos);
	title_.SetScale(Vector2(1.3f, 1.3f));
	//UI初期化
	uiSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("AButton"));
	Vector2 uiPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.15f,
	};
	uiSprite_.SetPos(uiPos);
	uiSprite_.SetSplitNum(Vector2(2,1));
	Vector2 texSize = {
		uiSprite_.GetTexture().size_.x / 2.f,
		uiSprite_.GetTexture().size_.y,
	};
	uiSprite_.SetTex_Size(texSize);
	
	//フィールドの初期化
	FieldManager::GetInstance()->Initialize();
	//カメラの初期化
	CameraManager::GetInstance()->Initialize();
	//タイトルロゴ上下移動のタイマー初期化
	titleFloatTimer_.SetLimitTime(240);
	//UIのアニメーションタイマー
	uiFloatTimer_.SetLimitTime(40);
	//背景
	backGround_.SetTexture(TextureManager::GetInstance()->GetTexture("Background"));
	Vector2 backgroundPos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f,
	};
	backGround_.SetPos(backgroundPos);

	sound_->AllBGMStop();
	sound_->Play("TitleBGM", true);

	Tutorial::GetInstance()->InitTutorial();
}

void TitleScene::Update()
{
#ifdef DEBUG
	//カメラ更新
	debugCamera_.Update();
	//Camera::scurrent_ = debugCamera_.GetCamera();
#endif // DEBUG
	Camera::scurrent_ = CameraManager::GetInstance()->GetCamera();
	Camera::scurrent_->Update(CameraMode::LookAT);

	CameraManager::GetInstance()->Update();

	lightManager_->Update();
	//コントローラー
	if (Controller::GetActive())
	{
		isSceneChange_ = Controller::GetTriggerButtons(PAD::INPUT_A);
		if (Controller::GetTriggerButtons(PAD::INPUT_A))
		{
			isSceneChange_ = true;
			SoundManager::Play("DecisionSE");
		}
	}
	//キーボード
	else
	{
		if ((Key::TriggerKey(DIK_RETURN) || Key::TriggerKey(DIK_SPACE)))
		{
			isSceneChange_ = true;
			SoundManager::Play("DecisionSE");
		}
	}

	if (isSceneChange_)
	{
		SceneManager::SetChangeStart(SceneName::Game);
	}

	FieldManager::GetInstance()->TitleUpdate();
	//タイトルロゴの上下移動更新
	titleFloatTimer_.AddTime();
	if (titleFloatTimer_.GetIsEnd()) {
		titleFloatTimer_.Reset();
	}
	float posY = UpAndDown(titleFloatTimer_.GetLimitTimer(), 30.f, titleFloatTimer_.GetTimer());
	Vector2 titlePos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 2.f + posY,
	};
	title_.SetPos(titlePos);

	//UIのアニメーション更新
	uiFloatTimer_.AddTime();
	if (uiFloatTimer_.GetIsEnd()) {
		uiFloatTimer_.Reset();
		uiAnimationIndex_++;
		if (uiAnimationIndex_ > 1) uiAnimationIndex_ = 0;
	}
	
	Vector2 texLeftTop = {
		uiSprite_.GetTexture().size_.x / 2.f,
		0,
	};
	texLeftTop.x *= uiAnimationIndex_;
	uiSprite_.SetTex_LeftTop(texLeftTop);
	uiSprite_.SetScale(Vector2(0.2f, 0.4f));

	title_.Update();
	uiSprite_.Update();
	backGround_.Update();
}

void TitleScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	backGround_.Draw();
	/////////////////
	//3Dオブジェクト//
	/////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	FieldManager::GetInstance()->Draw();
	/////////////////
	//スプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	title_.Draw();
	uiSprite_.Draw();
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawPostEffect()
{
}
