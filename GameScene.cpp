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
	Sprite::StaticIni();
	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();


	//天球
	skyDome = Object3d::CreateOBJ_uniptr("skydome");
	skyDomepos.Ini();
	skyDomepos.SetPosition(0, 0, 0);
	skyDomepos.scale = { 1,1,1 };

	cubeObj = Object3d::CreateOBJ_uniptr("testObj");
	cubePos.Ini();
	cubePos.SetPosition(0, 0, 0);
	cubePos.scale = { 1,1,1 };

	gameCamera.Ini();
	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	useVP = &gameCamera;
	//useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();

	sprite_.Ini();
	texture_ = textureM->LoadGraph("uv.png");
	sprite_.SetAnchor(0.5f, 0.5f);
	sprite_.SetPos(0, 0);
	sprite_.SetScale(0.5f, 0.5f);

	sprite2_.Ini("alphaBlend");
	sprite2_.SetAnchor(0, 0);
	sprite2_.SetPos(150, 150);
	sprite2_.SetScale(0.5f, 0.5f);

	isNext_ = 2;
	SceneManager::SetChangeStart(SceneNum::STAGE1_);

	//OutputDebugStringA("テスト文字出力");

	testSound_ = sound_->LoadWave("gumishipBGM.wav", "bgm");
	
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

	//カメラ
	if (input_->PushKey(DIK_A)) {
		gameCamera.eye.x -= 0.3f;
		gameCamera.target.x -= 0.3f;
	}
	if (input_->PushKey(DIK_D)) {
		gameCamera.eye.x += 0.3f;
		gameCamera.target.x += 0.3f;
	}
	if (input_->PushKey(DIK_S)) {
		gameCamera.eye.y -= 0.3f;
		gameCamera.target.y -= 0.3f;
	}
	if (input_->PushKey(DIK_W)) {
		gameCamera.eye.y += 0.3f;
		gameCamera.target.y += 0.3f;
	}

	//スプライト移動
	if (input_->PushKey(DIK_LEFT)) {
		cubePos.position.x -= 0.3f;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		cubePos.position.x += 0.3f;
	}
	if (input_->PushKey(DIK_UP)) {
		cubePos.position.y += 0.3f;
	}
	if (input_->PushKey(DIK_DOWN)) {
		cubePos.position.y -= 0.3f;
	}

	sprite_.SetFlipX(input_->PushKey(DIK_N));
	sprite_.SetFlipY(input_->PushKey(DIK_M));
	sprite_.SetInvisivle(input_->PushKey(DIK_B));
	sprite_.SetImGui(!input_->PushKey(DIK_B));

	sprite2_.SetInvisivle(input_->PushKey(DIK_V));
	sprite2_.SetImGui(!input_->PushKey(DIK_V));
#endif // _DEBUG
	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();
	cameraUpdate();

	//サウンド
	if (input_->TriggerKey(DIK_P)) {
		SoundManager::Play(testSound_, true, 1.0f);
	}

	if (input_->TriggerKey(DIK_L)) {
		SoundManager::Stop(testSound_);
	}





	skyDomepos.Update(*useVP);

	cubePos.Update(*useVP);
}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw();
	Object3d::SetBlend(BLEND_NORMAL);
	//天球
	skyDome->DrawOBJ(&skyDomepos);

	//Object3d::SetBlend(BLEND_ALPHA);
	//グミシップ
	cubeObj->DrawOBJ(&cubePos);


	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw();


	////////////
	//スプライト//
	////////////
	Sprite::PreDraw();

	Sprite::SetBlend(BLEND_NORMAL);
	sprite_.Draw(texture_);
	Sprite::SetBlend(BLEND_ALPHA);
	sprite2_.Draw(texture_);
}



void GameScene::CheckAllCollision()
{

}

void GameScene::cameraUpdate()
{

}

