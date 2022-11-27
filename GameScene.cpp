#include <random>
#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
GameScene::~GameScene()
{
}

void GameScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	Object3d::Ini();
	BoardObject::Ini();
	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();

	
	//天球
	skyDome = Object3d::CreateOBJ_uniptr("skydome");
	skyDomepos.Ini();
	skyDomepos.SetPosition(0, 0, 0);
	skyDomepos.scale = { 1,1,1 };

	gumiship = Object3d::CreateOBJ_uniptr("gumiShip");
	gumishippos.Ini();
	gumishippos.SetPosition(0, 0, 0);
	gumishippos.scale = { 1,1,1 };

	gameCamera.Ini();
	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();


	isNext_ = 2;
	SceneManager::SetChangeStart(SceneNum::STAGE1_);
}

void GameScene::Update()
{
#ifdef _DEBUG
	//タイトルシーンへ移動
	if (input_->TriggerKey(DIK_0)) {
		isNext_ = true;
		SceneManager::SetChangeStart(SceneNum::TITLE_);
	}

	if (input_->TriggerKey(DIK_1)) {
		if (useVP == debugCamera.GetViewProjection())useVP = &gameCamera;
		else if (useVP == &gameCamera)useVP = debugCamera.GetViewProjection();

	}

	if (input_->PushKey(DIK_J)) {
		gameCamera.eye.x -= 0.2f;
	}

	if (input_->PushKey(DIK_K)) {
		gameCamera.eye.x += 0.2f;
	}

#endif // _DEBUG
	//カメラ更新
	debugCamera.Update();
	gameCamera.Update();
	cameraUpdate();

	skyDomepos.Update(*useVP);

	gumishippos.Update(*useVP);
}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw();

	Object3d::SetBlend(BLEND_ALPHA);
	//天球
	skyDome->DrawOBJ(&skyDomepos);

	Object3d::SetNormal();

	
	//グミシップ
	gumiship->DrawOBJ(&gumishippos);
	Object3d::SetNormal();

	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw();
	

	////////////
	//スプライト//
	////////////
	
}



void GameScene::CheckAllCollision()
{

}

void GameScene::cameraUpdate()
{
	
}

