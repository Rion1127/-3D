#include <random>
#include "ParticleScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"

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

	Object3d::Ini();
	//BoardObject::Ini();
	Sprite::StaticIni();
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
	////////////////
	//3D�I�u�W�F�N�g//
	////////////////
	Object3d::PreDraw();
	Object3d::SetBlend(BLEND_NORMAL);

	

	ParticleManager::PreDraw();

	particle_.Draw();

	///////////////////
	//��RD�I�u�W�F�N�g//
	///////////////////


	////////////
	//�X�v���C�g//
	////////////
	Sprite::PreDraw();
	//testSprite_.Draw(uvtexture_);
	//testSprite_.DrawImGui();
	Sprite::SetBlend(BLEND_ALPHA);
	//testSprite2_.Draw(uvtexture_);
	//testSprite2_.DrawImGui();
}

