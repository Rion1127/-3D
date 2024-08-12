#include "GameOverScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "Camera2d.h"
#include "Easing.h"
#include "Timer.h"
#include "mSound.h"

/**
 * @file ClearScene.cpp
 * @brief メインのゲームシーンクラス
 */

GameOverScene::~GameOverScene()
{
}

void GameOverScene::Init()
{
	sound_ = SoundManager::GetInstance();

	isSceneChange_ = false;
}

void GameOverScene::Update()
{

	//コントローラー
	if (Controller::GetActive())
	{
		isSceneChange_ = Controller::GetTriggerButtons(PAD::INPUT_A);
	}
	//キーボード
	else
	{
		isSceneChange_ = (Key::TriggerKey(DIK_RETURN) || Key::TriggerKey(DIK_SPACE));
	}

	if (isSceneChange_)
	{
		SceneManager::SetChangeStart(SceneName::Title);
	}
#ifdef _DEBUG

	if (Key::TriggerKey(DIK_G))
	{
		SceneManager::Transition<GameScene>();
	}

#endif // _DEBUG
}

void GameOverScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	//Camera2d::GetInstance()->DrawCol();
	////////////////
	//3Dオブジェクト//
	////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);

	///////////////////
	//フロントスプライト//
	//////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	PipelineManager::PreDraw("Particle", POINTLIST);

}

void GameOverScene::DrawRenderTexture()
{

}

void GameOverScene::DrawPostEffect()
{
	PipelineManager::PreDraw("Particle", POINTLIST);
}

void GameOverScene::CameraUpdate()
{
}