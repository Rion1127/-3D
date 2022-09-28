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
#include "GameScene.h"
///


#include <DirectXTex.h>


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	//winAPI初期化
	WinAPI* winApi = WinAPI::GetInstance();
	winApi->Ini();

	//DirectX初期化
	DirectXCommon* directX = DirectXCommon::GetInstance();
	directX->Ini(winApi);
	//テクスチャマネージャー初期化
	TextureManager::GetInstance()->Ini(directX->GetDevice());
	//インプット初期化
	DirectXInput::InputIni(winApi->w, winApi->hwnd);
	//ゲームシーン初期化
	GameScene* gameScene = new GameScene;
	gameScene->Ini();

	///////////////////////
	//描画初期化処理　ここまで//
	///////////////////////

	Controller* controller = Controller::GetInstance();
	controller->Ini();

	// ゲームループ
	while (true) {
		if (winApi->MsgCheck()) {
			break;
		}

		DirectXInput::InputUpdata();
		controller->Update();
		//////////////////////
		//プログラム記入ここから//
		//////////////////////

		//ゲームシーン更新
		gameScene->Updata();
		
		///////////////////////
		// 4.描画コマンドここから//
		///////////////////////
		directX->PreDraw();

		gameScene->Draw();

		directX->PostDraw();
		///////////////////////
		// 4.描画コマンドここまで//
		///////////////////////
	}
	// ウィンドウクラスを登録解除
	winApi->ReleaseClass();

	delete gameScene;

	return 0;
}
