#include "Texture.h"
#include <random>
#include "GameScene.h"

GameScene::~GameScene()
{
	delete gumiShip;
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
	
	debugCamera.DebugCameraIni();

	gumiShip = Object3d::CreateOBJ("gumiShip");
	worldTransform_.Ini();

	skyDome = Object3d::CreateOBJ("skydome");
	skyDomepos.Ini();
	skyDomepos.scale = { 1,1,1 };

}

void GameScene::Updata()
{
#ifdef _DEBUG


#endif // _DEBUG

	debugCamera.Update();
	
	gumiShip->ChangeColor(0, 0, 0, 0);

	skyDomepos.Update(debugCamera.GetViewProjection());

	worldTransform_.Update(debugCamera.GetViewProjection());

}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw(directX->GetCommandList());

	gumiShip->DrawOBJ(&worldTransform_);

	skyDome->DrawOBJ(&skyDomepos);
	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw(directX->GetCommandList());

	////////////
	//スプライト//
	////////////
	
}
