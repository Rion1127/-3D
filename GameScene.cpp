#include "Texture.h"
#include <random>
#include "GameScene.h"

GameScene::~GameScene()
{
	delete skyDome;
	delete gumiShipObj;
}

void GameScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();

	Object3d::Ini(directX->GetDevice());
	BoardObject::Ini(directX->GetDevice());

	debugCamera.DebugCameraIni(&winApi_->hwnd);

	marioGraph = textureM->LoadGraph("Resources/mario.jpg");
	khGraph = textureM->LoadGraph("Resources/KH.jpg");
	enemyGraph = textureM->LoadGraph("Resources/enemy.jpg");
	keyBladeGraph = textureM->LoadGraph("Resources/keyBlade2.png");
	gumishipGraph = textureM->LoadGraph("Resources/gumiship.png");
	graph = marioGraph;

	skyDome = Object3d::CreateOBJ("skydome");
	skyDomePos.Initialize();
	skyDomePos.SetPosition(0, 0, 0);
	skyDomePos.SetRotation(0, 0, 0);
	skyDomePos.SetScale(5, 5, 5);

	gumiShipObj = Object3d::CreateOBJ("sphere");
	gumiShipPos.Initialize();
	gumiShipPos.SetPosition(0, 0, 0);
	gumiShipPos.SetRotation(0, 0, 0);
	gumiShipPos.SetScale(1, 1, 1);


	sound_->LoadWave("result.wav", "ResultBGM");

	boardPos.Initialize();
	boardPos.SetPosition(0, 0, 0);
	boardPos.SetRotation(ConvertAngleToRadian(90),0,0);
	
	viewProjection.SetEyePos(0, 0, -200);
	viewProjection.SetTarget(0, 0, 0);
	viewProjection.SetUpVec(0, 1, 0);
	viewProjection.Ini();

	
}

void GameScene::Updata()
{
#ifdef _DEBUG


#endif // _DEBUG
	if (input_->TriggerKey(DIK_1)) {
		sound_->Play("ResultBGM",0.05f);
	}
	if (input_->TriggerKey(DIK_2)) {
		sound_->Stop("ResultBGM");
	}

	debugCamera.Update(winApi_->hwnd);

	boardPos.UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection()/*railCamera.viewProjection*/);


	float speed = 0.5f;
	if (input_->PushKey(DIK_W)) {
		gumiShipPos.position += { 0,speed,0 };
	}
	if (input_->PushKey(DIK_S)) {
		gumiShipPos.position += { 0,-speed,0 };
	}
	if (input_->PushKey(DIK_A)) {
		gumiShipPos.position += { -speed,0,0 };
	}
	if (input_->PushKey(DIK_D)) {
		gumiShipPos.position += { speed,0,0 };
	}

	gumiShipPos.UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection());
	skyDomePos.UpdateObject3d(debugCamera.GetViewProjection());

}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////

	Object3d::PreDraw(directX->GetCommandList());



	//player_.Draw(gumishipGraph);
	//enemy_.Draw();

	skyDome->DrawOBJ(&skyDomePos);
	gumiShipObj->DrawOBJ(&gumiShipPos);
	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw(directX->GetCommandList());

	boardObject.Draw(&boardPos, khGraph);
	////////////////
	//2Dオブジェクト//
	////////////////

}
