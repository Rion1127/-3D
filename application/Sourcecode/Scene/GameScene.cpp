#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "Camera2d.h"
#include "ClearScene.h"
#include <filesystem>
#include "mSound.h"
#include "Framework.h"

/**
 * @file GameScene.cpp
 * @brief メインのゲームシーンクラス
 */

void GameScene::Init()
{
	Camera2d::GetInstance()->Init();
}

void GameScene::Update()
{
#ifdef _DEBUG
	
#endif // _DEBUG
	
}

void GameScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	
	///////////////////
	//フロントスプライト//
	//////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
}

void GameScene::DrawRenderTexture()
{
}

void GameScene::DrawPostEffect()
{
	PipelineManager::PreDraw("Particle", POINTLIST);
}