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

	//�f�o�b�O�J����������
	debugCamera.DebugCameraIni();
	debugCamera.Update();

	gameCamera.SetEyePos(Vector3(0, 8, -20));
	gameCamera.SetTarget(Vector3(0, 0, 0));
	gameCamera.Update();
	//useVP = &gameCamera;
	useVP = debugCamera.GetViewProjection();
	useVP->SetOriginalPos();
	

	
	WT_.Ini();

	texture_ = textureM->LoadGraph("keyBlade2.png");
	whiteTexture_ = textureM->LoadGraph("uv.png");
}

void ParticleScene::Update()
{
	//�J�����X�V
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();


	WT_.Update(*useVP);

	object_.Update(*useVP);

}

void ParticleScene::Draw()
{

	///////////////////
	//��RD�I�u�W�F�N�g//
	///////////////////
	Particle::PreDraw();

	object_.Draw(whiteTexture_);
}
