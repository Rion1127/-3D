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

	//�f�o�b�O�J����������
	debugCamera.DebugCameraIni();

	gameCamera.Ini();
	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();
}

void ParticleScene::Update()
{
	//�J�����X�V
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();

}

void ParticleScene::Draw()
{

	///////////////////
	//��RD�I�u�W�F�N�g//
	///////////////////
	BoardObject::PreDraw();

}
