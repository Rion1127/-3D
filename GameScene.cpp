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
	//�f�o�b�O�J����������
	debugCamera.DebugCameraIni();

	
	//�V��
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

	sprite_.Ini();
	texture_ = textureM->LoadGraph("uv.png");
	sprite_.SetAnchor(0.5f, 0.5f);
	sprite_.SetPos(0,0);
	sprite_.SetScale(0.5f, 0.5f);

	sprite2_.Ini("alphaBlend");
	sprite2_.SetAnchor(0, 0);
	sprite2_.SetPos(150,150);
	sprite2_.SetScale(0.5f, 0.5f);

	isNext_ = 2;
	SceneManager::SetChangeStart(SceneNum::STAGE1_);

	OutputDebugStringA("�e�X�g�����o��");
}

void GameScene::Update()
{
#ifdef _DEBUG
	//�^�C�g���V�[���ֈړ�
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

	//�X�v���C�g�ړ�
#pragma region
	if (input_->PushKey(DIK_UP)) {
		Vector2 pos = sprite_.GetPos();
		pos += {0, -2};
		sprite_.SetPos(pos);
	}

	if (input_->PushKey(DIK_DOWN)) {
		Vector2 pos = sprite_.GetPos();
		pos += {0, 2};
		sprite_.SetPos(pos);
	}

	if (input_->PushKey(DIK_RIGHT)) {
		Vector2 pos = sprite_.GetPos();
		pos += {2, 0};
		sprite_.SetPos(pos);
	}

	if (input_->PushKey(DIK_LEFT)) {
		Vector2 pos = sprite_.GetPos();
		pos += {-2, 0};
		sprite_.SetPos(pos);
	}
#pragma endregion

	sprite_.SetFlipX(input_->PushKey(DIK_N));
	sprite_.SetFlipY(input_->PushKey(DIK_M));
	sprite_.SetInvisivle(input_->PushKey(DIK_B));
	sprite_.SetImGui(!input_->PushKey(DIK_B));

	sprite2_.SetInvisivle(input_->PushKey(DIK_V));
	sprite2_.SetImGui(!input_->PushKey(DIK_V));
#endif // _DEBUG
	//�J�����X�V
	if (input_->PushKey(DIK_LCONTROL)) {
		debugCamera.Update();
	}
	gameCamera.Update();
	cameraUpdate();








	skyDomepos.Update(*useVP);

	gumishippos.Update(*useVP);
}

void GameScene::Draw()
{
	////////////////
	//3D�I�u�W�F�N�g//
	////////////////
	Object3d::PreDraw();
	Object3d::SetBlend(BLEND_NORMAL);
	//�V��
	skyDome->DrawOBJ(&skyDomepos);

	//Object3d::SetBlend(BLEND_ALPHA);
	//�O�~�V�b�v
	gumiship->DrawOBJ(&gumishippos);
	

	///////////////////
	//��RD�I�u�W�F�N�g//
	///////////////////
	BoardObject::PreDraw();
	

	////////////
	//�X�v���C�g//
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

