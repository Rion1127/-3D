#include "Texture.h"
#include <random>
#include "GameScene.h"

GameScene::~GameScene()
{
	
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
	
}

void GameScene::Updata()
{
#ifdef _DEBUG


#endif // _DEBUG
	
}

void GameScene::Draw()
{
	////////////////
	//3Dオブジェクト//
	////////////////
	Object3d::PreDraw(directX->GetCommandList());


	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw(directX->GetCommandList());

	////////////////
	//2Dオブジェクト//
	////////////////
	
}
