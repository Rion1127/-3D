#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "ClearScene.h"
#include "mSound.h"

/**
 * @file GameScene.cpp
 * @brief メインのゲームシーンクラス
 */

GameScene::~GameScene(void)
{
	ParticleManager::GetInstance()->AllClear();
}

void GameScene::Init()
{
	sceneName_ = "GameScene";

	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLight(lightManager_->GetLightGroup());
	//////////////////////////////////
	//3Dオブジェクト
	object3d_ = std::move(std::make_unique<Object3d>());
	//モデル読み込み
	//object3d_->SetModel(Model::CreateOBJ_uniptr("field", true, false));
	//////////////////////////////////

	SoundManager::AllBGMStop();
	//SoundManager::Play("GameplayBGM", true);
	
}

void GameScene::Update()
{
#ifdef _DEBUG
	
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();
	Camera::scurrent_->Update(CameraMode::LookAT);
#endif // _DEBUG

	lightManager_->Update();
	colliderManager_.Update();

	ParticleManager::GetInstance()->Update();
	
#ifdef _DEBUG
	if (Framework::isImguiDisplay_) {
		
	}
#endif // _DEBUG
}

void GameScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	
	/////////////////
	//3Dオブジェクト//
	/////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);

	//タイマーゲージはフィールドの一層上に描画
	PipelineManager::PreDraw("CircleGauge", TRIANGLELIST);

	
	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	
	ParticleManager::GetInstance()->Draw();
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