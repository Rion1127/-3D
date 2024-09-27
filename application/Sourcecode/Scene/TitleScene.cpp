#include "TitleScene.h"
#include "SceneManager.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	sceneName_ = "TitleScene";

	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	sound_ = SoundManager::GetInstance();
}

void TitleScene::Update()
{
#ifdef DEBUG
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();
#endif // DEBUG
	Camera::scurrent_->Update(CameraMode::LookAT);

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
}

void TitleScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	
	/////////////////
	//3Dオブジェクト//
	/////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	
	/////////////////
	//スプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawPostEffect()
{
}
