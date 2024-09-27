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

#include <fstream>
#include <iostream>

/**
 * @file ClearScene.cpp
 * @brief メインのゲームシーンクラス
 */

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
	sceneName_ = "ClearScene";

	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	sound_ = SoundManager::GetInstance();

	SoundManager::AllStop();

	//SoundManager::Play("ClearSE");
}

void ClearScene::Update()
{
#ifdef _DEBUG

#endif // _DEBUG
	lightManager_->Update();

	Rank();

	//コントローラー
	if (Controller::GetActive()) {
		isSceneChange_ = Controller::GetTriggerButtons(PAD::INPUT_A);
		if (Controller::GetTriggerButtons(PAD::INPUT_A)) {
			isSceneChange_ = true;
			SoundManager::Play("DecisionSE");
		}
	}
	//キーボード
	else {
		if ((Key::TriggerKey(DIK_RETURN) || Key::TriggerKey(DIK_SPACE))) {
			isSceneChange_ = true;
			SoundManager::Play("DecisionSE");
		}
	}

	if (isSceneChange_) {
		isSceneChange_ = false;
		SceneManager::SetChangeStart(SceneName::Game);
	}

#ifdef _DEBUG
	if (Framework::isImguiDisplay_) {
		
	}
#endif // _DEBUG
}

void ClearScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
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

void ClearScene::DrawRenderTexture()
{

}

void ClearScene::DrawPostEffect()
{
	PipelineManager::PreDraw("Particle", POINTLIST);
}