#include "Texture.h"
#include <random>
#include "DirectXInput.h"
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
	

	Object3d::Ini(directX->GetDevice());
	BoardObject::Ini(directX->GetDevice());
	Obj2D.Ini(directX->GetDevice());

	debugCamera.DebugCameraIni(winApi_->hwnd);

	marioGraph = TextureManager::GetInstance()->LoadGraph("Resources/mario.jpg");
	khGraph = TextureManager::GetInstance()->LoadGraph("Resources/KH.jpg");
	enemyGraph = TextureManager::GetInstance()->LoadGraph("Resources/enemy.jpg");
	keyBladeGraph = TextureManager::GetInstance()->LoadGraph("Resources/keyBlade2.png");
	gumishipGraph = TextureManager::GetInstance()->LoadGraph("Resources/gumiship.png");
	graph = marioGraph;

	skyDome = Object3d::CreateOBJ("skydome", directX->GetDevice());
	gumiShipObj = Object3d::CreateOBJ("gumiShip", directX->GetDevice());

	
	boardPos.SetPosition(20, 0, 0);
	boardPos.InitializeObject3d();

	//�����V�[�h������
	std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	std::mt19937_64 engine(seed_gen());

	//�z����̑S�I�u�W�F�N�g�ɑ΂���
	for (int i = 0; i < _countof(worldTransform); i++) {
		//�����͈͂̎w��
		std::uniform_real_distribution<float> transDistX(-50, 50);
		std::uniform_real_distribution<float> transDistY(-50, 50);
		std::uniform_real_distribution<float> transDistZ(-50, 50);

		//�����G���W����n���A�w��͈͂��烉���_���Ȑ��l�𓾂�
		float x = transDistX(engine);
		float y = transDistY(engine);
		float z = transDistZ(engine);

		//�������火�͐e�q�\���̃T���v��
		//�퓬�ȊO�Ȃ�
		worldTransform[i].InitializeObject3d();
		if (i > 0) {
			//�ЂƂO�̃I�u�W�F�N�g��e�I�u�W�F�N�g�Ƃ���
			worldTransform[i].parent = &worldTransform[i - 1];
			//�e�I�u�W�F�N�g��9���̑傫��
			worldTransform[i].SetScale(1, 1, 1);
			//�e�I�u�W�F�N�g�ɑ΂���Z�������30�x��]
			worldTransform[i].SetRotation(0.0f, 0.0f, 0);
			//�e�I�u�W�F�N�g�ɑ債��Z����-8.0���炷
			worldTransform[i].SetPosition(0, 0, 15);
		}
		else {
			worldTransform[i].SetPosition(0, -15, 0);
		}
	}
}

void GameScene::Updata()
{
	debugCamera.Update(winApi_->hwnd);

	//player_.Update(device.Get(),debugCamera.GetViewProjection()/*viewProjection*//*railCamera.viewProjection*/);
		//enemy_.Update(/*viewProjection*/debugCamera.GetViewProjection());

	boardPos.UpdateObject3d(debugCamera.GetViewProjection()/*railCamera.viewProjection*/);

	for (int i = 0; i < _countof(worldTransform); i++) {
		worldTransform[i].UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection()/*railCamera.viewProjection*/);
	}

#pragma region �F�ω�
	if (DirectXInput::IsKeyTrigger(DIK_0)) {
		if (isChangeColor == false) isChangeColor = true;
		else if (isChangeColor == true) isChangeColor = false;
	}

	if (isChangeColor == true) {
		if (objectColor.y >= 0) {
			objectColor.x += 0.05f;
		}
		else if (objectColor.y <= 0) {
			objectColor.x -= 0.05f;
		}

		if (objectColor.z <= 0) {
			objectColor.y -= 0.05f;
		}
		else if (objectColor.z >= 0) {
			objectColor.y += 0.05f;
		}

		if (objectColor.x >= 0) {
			objectColor.z -= 0.05f;
		}
		else if (objectColor.x <= 0) {
			objectColor.z += 0.05f;
		}

		if (objectColor.x > 1) {
			objectColor.x = 1;
		}
		if (objectColor.y > 1) {
			objectColor.y = 1;
		}
		if (objectColor.z > 1) {
			objectColor.z = 1;
		}

		model_.ChangeColor(objectColor);
	}
#pragma endregion

}

void GameScene::Draw()
{
	////////////////
	//3D�I�u�W�F�N�g//
	////////////////

	Object3d::PreDraw(directX->GetCommandList());
	BoardObject::PreDraw(directX->GetCommandList());


	//player_.Draw(gumishipGraph);
	//enemy_.Draw();
	for (int i = 0; i < _countof(worldTransform); i++) {
		model_.DrawCube(&worldTransform[i], marioGraph);
	}

	skyDome->DrawOBJ(&worldTransform[0]);
	gumiShipObj->DrawOBJ(&worldTransform[0]);

	boardObject.Draw(&boardPos, khGraph);
	////////////////
	//2D�I�u�W�F�N�g//
	////////////////
	Vector2 pos = { 100,200 };

	Obj2D.PreDraw(directX->GetCommandList());
	Obj2D.Draw(pos, keyBladeGraph);
}