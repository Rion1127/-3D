#include "ParticleScene.h"

ParticleScene::~ParticleScene()
{
}

void ParticleScene::Ini()
{
	directX = DirectXCommon::GetInstance();
	winApi_ = WinAPI::GetInstance();
	input_ = DirectXInput::GetInstance();
	controller_ = Controller::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = SoundManager::GetInstance();


	BoardObject::Ini();

	//デバッグカメラ初期化
	debugCamera.DebugCameraIni();

	gameCamera.Ini();
	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();

	
	WT_.Ini();

	texture_ = textureM->LoadGraph("keyBlade2.png");
	whiteTexture_ = textureM->LoadGraph("white1×1.png");
}

void ParticleScene::Update()
{
	//カメラ更新
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();


	WT_.Update(*useVP);

}

void ParticleScene::Draw()
{

	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw();

	object_.Draw(&WT_, whiteTexture_);
}
