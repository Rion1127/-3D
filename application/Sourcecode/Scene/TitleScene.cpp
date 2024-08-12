#include "TitleScene.h"
#include "SceneManager.h"

TitleScene::~TitleScene()
{
}

void TitleScene::Init()
{
	sound_ = SoundManager::GetInstance();
	SoundManager::AllStop();
}

void TitleScene::Update()
{
	//コントローラー
	if (Controller::GetActive())
	{
		isSceneChange_ = Controller::GetTriggerButtons(PAD::INPUT_A);
		if (Controller::GetTriggerButtons(PAD::INPUT_A))
		{
			isSceneChange_ = true;
			//SoundManager::Play("PauseSelectSE");
		}
	}
	//キーボード
	else
	{
		if ((Key::TriggerKey(DIK_RETURN) || Key::TriggerKey(DIK_SPACE)))
		{
			isSceneChange_ = true;
			//SoundManager::Play("PauseSelectSE");
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
}

void TitleScene::DrawRenderTexture()
{
}

void TitleScene::DrawPostEffect()
{
}
