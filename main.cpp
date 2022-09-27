#include <Windows.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3d12.h>
#include <dxgi1_6.h>
#include <cassert>
#include <vector>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
#include <wrl.h>
using namespace Microsoft::WRL;
///
#include "DirectXInput.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Texture.h"
#include "DebugCamera.h"
#include "Object3d.h"
#include "Object2D.h"
#include "ViewProjection.h"
//#include "Player.h"
#include "PlayerBullet.h"
//#include "Enemy.h"
#include "Collision.h"
#include "boardObject.h"
#include "RailCamera.h"
#include "Model.h"
#include "WinAPI.h"
#include "DirectX.h"
///
#include <random>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXTex.h>


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//winAPI初期化
	WinAPI* winApi = WinAPI::GetInstance();
	winApi->Ini();

	//DirectX初期化
	DirectXCommon* directX = DirectXCommon::GetInstance();
	directX->Ini(winApi);

	TextureManager::GetInstance()->Ini(directX->GetDevice());

	uint32_t marioGraph, khGraph,/*enemyGraph,*/keyBladeGraph,colorGraph,gumishipGraph;
	uint32_t graph;
	marioGraph = TextureManager::GetInstance()->LoadGraph("Resources/mario.jpg");
	khGraph = TextureManager::GetInstance()->LoadGraph("Resources/KH.jpg");
	//enemyGraph = TextureManager::GetInstance()->LoadGraph("Resources/enemy.jpg");
	keyBladeGraph = TextureManager::GetInstance()->LoadGraph("Resources/keyBlade2.png");
	colorGraph = TextureManager::GetInstance()->LoadGraph("Resources/coloring2-1.png");
	gumishipGraph = TextureManager::GetInstance()->LoadGraph("Resources/gumiship.png");
	graph = marioGraph;

	Object3d model_;
	Object3d::Ini(directX->GetDevice());

	BoardObject boardObject;
	BoardObject::Ini(directX->GetDevice());

	Object2D Obj2D;
	Obj2D.Ini(directX->GetDevice());


	///////////////////////
	//描画初期化処理　ここまで//
	///////////////////////
	DirectXInput::InputIni(winApi->w,winApi->hwnd);
	
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//3Dオブジェクトの数
	const size_t kObjectCount = 20;
	//3Dオブジェクトの配列
	WorldTransform worldTransform[kObjectCount];
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
			worldTransform[i].SetScale(1,1,1);
			//親オブジェクトに対してZ軸周りに30度回転
			worldTransform[i].SetRotation(0.0f,0.0f,0);
			//親オブジェクトに大したZ方向-8.0ずらす
			worldTransform[i].SetPosition(0,0, 15);
		}
		else {
			worldTransform[i].SetPosition(0, -15, 0);
		}
	}
	//ボードオブジェクト
	WorldTransform boardPos;
	boardPos.SetPosition(20, 0, 0);
	boardPos.InitializeObject3d();


	DebugCamera debugCamera;
	debugCamera.DebugCameraIni(winApi->hwnd);


	//オブジェクト色変更
	bool isChangeColor = false;
	XMFLOAT4 objectColor{1,1,1,1};

	Controller* controller = Controller::GetInstance();
	controller->Ini();

	//Player player_;
	//player_.Ini(device.Get());
	//Enemy enemy_;
	//enemy_.Ini(device.Get());
	RailCamera railCamera;
	Vector3 pos = { 0,0,-1 };
	Vector3 rot = { 0,0,0 };

	Object3d* skyDome = nullptr;
	skyDome = Object3d::CreateOBJ("skydome", directX->GetDevice());
	Object3d* gumiShipObj = nullptr;
	gumiShipObj = Object3d::CreateOBJ("gumiShip", directX->GetDevice());
	// ゲームループ
	while (true) {
		if (winApi->MsgCheck()) {
			break;
		}

		DirectXInput::InputUpdata();
		debugCamera.Update(winApi->hwnd);
		controller->Update();
		//////////////////////
		//プログラム記入ここから//
		//////////////////////
		
		//player_.Update(device.Get(),debugCamera.GetViewProjection()/*viewProjection*//*railCamera.viewProjection*/);
		//enemy_.Update(/*viewProjection*/debugCamera.GetViewProjection());

		boardPos.UpdateObject3d(debugCamera.GetViewProjection()/*railCamera.viewProjection*/);

		for (int i = 0; i < _countof(worldTransform); i++) {
			worldTransform[i].UpdateObject3d(/*viewProjection*/debugCamera.GetViewProjection()/*railCamera.viewProjection*/);
		}

#pragma region 色変化
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

		///////////////////////
		// 4.描画コマンドここから//
		///////////////////////
		directX->PreDraw();

		////////////////
		//3Dオブジェクト//
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
		//2Dオブジェクト//
		////////////////
		Vector2 pos = {100,200};

		Obj2D.PreDraw(directX->GetCommandList());
		Obj2D.Draw(pos,keyBladeGraph);
		
		///////////////////////
		// 4.描画コマンドここまで//
		///////////////////////
		directX->PostDraw();
	}

	// ウィンドウクラスを登録解除
	winApi->ReleaseClass();

	delete skyDome;
	delete gumiShipObj;

	return 0;
}
