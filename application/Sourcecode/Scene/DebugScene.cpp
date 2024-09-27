#include "DebugScene.h"
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

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
	sceneName_ = "DebugScene";

	lightManager_ = std::make_shared<LightManager>();
	Model::SetLight(lightManager_->GetLightGroup());
	AssimpModel::SetLight(lightManager_->GetLightGroup());
	Camera2d::GetInstance()->Init();

	testState_ = TestState::IDLE;
	assimpDrawer_.SetAnimationTimer(30);
	assimpDrawer_.SetRepeatAnimation(true);
	assimpDrawer_.Init(&testState_);
}

void DebugScene::Update()
{
#ifdef _DEBUG
	//カメラ更新
	debugCamera_.Update();
	Camera::scurrent_ = debugCamera_.GetCamera();
	Camera::scurrent_->Update(CameraMode::LookAT);
#endif // _DEBUG
	
	colliderManager_.Update();
	lightManager_->Update();

	assimpDrawer_.Update();

	ParticleManager::GetInstance()->Update();
}

void DebugScene::Draw()
{
	/////////////////
	//3Dオブジェクト//
	/////////////////
	PipelineManager::PreDraw("Object3D", TRIANGLELIST);
	
	PipelineManager::PreDraw("Object3D_NotCull", TRIANGLELIST);
	/////////////////
	//assimpオブジェクト//
	/////////////////
	PipelineManager::PreDraw("assimp", TRIANGLELIST);
	assimpDrawer_.Draw();
	
	ParticleManager::GetInstance()->Draw();
}

void DebugScene::DrawRenderTexture()
{
}

void DebugScene::DrawPostEffect()
{
}

void DebugScene::CallBack()
{
	
}
