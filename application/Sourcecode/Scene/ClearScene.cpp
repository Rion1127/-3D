#include "ClearScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "Camera2d.h"
#include "ClearScene.h"
#include "Easing.h"
#include "Timer.h"
#include "mSound.h"

/**
 * @file ClearScene.cpp
 * @brief メインのゲームシーンクラス
 */

ClearScene::~ClearScene()
{
}

void ClearScene::Init()
{
	sound_ = SoundManager::GetInstance();

	SoundManager::AllStop();

	SoundManager::Play("ClearSE");
}

void ClearScene::Update()
{
#ifdef _DEBUG

	if (Key::TriggerKey(DIK_G))
	{
		SceneManager::Transition<GameScene>();
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

void ClearScene::CameraUpdate()
{
}