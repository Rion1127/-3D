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

}

void GameScene::Updata()
{
#ifdef _DEBUG


#endif // _DEBUG

	debugCamera.Update();

	worldTransform_.Update(debugCamera.GetViewProjection());

}

void GameScene::Draw()
{
	////////////////
	//3D�I�u�W�F�N�g//
	////////////////
	Object3d::PreDraw(directX->GetCommandList());

	gumiShip->DrawOBJ(&worldTransform_);
	///////////////////
	//��RD�I�u�W�F�N�g//
	///////////////////
	BoardObject::PreDraw(directX->GetCommandList());

	////////////
	//�X�v���C�g//
	////////////
	
}
