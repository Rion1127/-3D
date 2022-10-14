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
	input_ = DirectXInput::GetInstance();
	textureM = TextureManager::GetInstance();
	sound_ = mSound::GetInstance();

	Object3d::Ini(directX->GetDevice());
	BoardObject::Ini(directX->GetDevice());
	Obj2D.Ini(directX->GetDevice());

	debugCamera.DebugCameraIni(&winApi_->hwnd);

	marioGraph = textureM->LoadGraph("Resources/mario.jpg");
	khGraph = textureM->LoadGraph("Resources/KH.jpg");
	enemyGraph = textureM->LoadGraph("Resources/enemy.jpg");
	keyBladeGraph = textureM->LoadGraph("Resources/keyBlade2.png");
	gumishipGraph = textureM->LoadGraph("Resources/gumiship.png");
	graph = marioGraph;

	skyDome = Object3d::CreateOBJ("skydome", directX->GetDevice());
	gumiShipObj = Object3d::CreateOBJ("gumiShip", directX->GetDevice());
	gumiShipPos.InitializeObject3d();
	gumiShipPos.SetPosition(0, 5, 0);
	gumiShipPos.SetRotation(0, 0, 0);
	gumiShipPos.SetScale(1, 1, 1);


	testSound = sound_->Load("gumishipBGM.wav");


	boardPos.SetPosition(20, 0, 0);
	boardPos.InitializeObject3d();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());

	//配列内の全オブジェクトに対して
	for (int i = 0; i < _countof(worldTransform); i++) {
		//乱数範囲の指定
		std::uniform_real_distribution<float> transDistX(-50, 50);
		std::uniform_real_distribution<float> transDistY(-50, 50);
		std::uniform_real_distribution<float> transDistZ(-50, 50);

		//乱数エンジンを渡し、指定範囲からランダムな数値を得る
		float x = transDistX(engine);
		float y = transDistY(engine);
		float z = transDistZ(engine);

		//ここから↓は親子構造のサンプル
		//戦闘以外なら
		worldTransform[i].InitializeObject3d();
		if (i > 0) {
			//ひとつ前のオブジェクトを親オブジェクトとする
			worldTransform[i].parent = &worldTransform[i - 1];
			//親オブジェクトの9割の大きさ
			worldTransform[i].SetScale(1, 1, 1);
			//親オブジェクトに対してZ軸周りに30度回転
			worldTransform[i].SetRotation(0.0f, 0.0f, 0);
			//親オブジェクトに大したZ方向-8.0ずらす
			worldTransform[i].SetPosition(0, 0, 15);
		}
		else {
			worldTransform[i].SetPosition(0, -15, 0);
		}
	}
}

void GameScene::Updata()
{
#ifdef _DEBUG

	if (sound_->isPlaying(testSound) == false) {
		sound_->Play(testSound, true, 0.5f);
	}


	if (input_->IsKeyTrigger(DIK_1)) {
		sound_->Stop(testSound);
	}

#endif // _DEBUG

	debugCamera.Update(winApi_->hwnd);

	boardPos.UpdateObject3d(debugCamera.GetViewProjection()/*railCamera.viewProjection*/);

	for (int i = 0; i < _countof(worldTransform); i++) {
		worldTransform[i].UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection()/*railCamera.viewProjection*/);
	}

	gumiShipPos.UpdateObject3d(debugCamera.GetViewProjection());

#pragma region 色変化
	if (input_->IsKeyTrigger(DIK_0)) {
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
	//3Dオブジェクト//
	////////////////

	Object3d::PreDraw(directX->GetCommandList());



	//player_.Draw(gumishipGraph);
	//enemy_.Draw();
	for (int i = 0; i < _countof(worldTransform); i++) {
		model_.DrawCube(&worldTransform[i], marioGraph);
	}

	skyDome->DrawOBJ(&worldTransform[0]);
	gumiShipObj->DrawOBJ(&gumiShipPos);
	///////////////////
	//板状３Dオブジェクト//
	///////////////////
	BoardObject::PreDraw(directX->GetCommandList());

	boardObject.Draw(&boardPos, khGraph);
	////////////////
	//2Dオブジェクト//
	////////////////
	Vector2 pos = { 100,200 };

	Obj2D.PreDraw(directX->GetCommandList());
	Obj2D.Draw(pos, keyBladeGraph);
}
