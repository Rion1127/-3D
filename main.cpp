#include <Windows.h>
///
#include "DirectXInput.h"
#include "Texture.h"
#include "Util.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "GameScene.h"
///

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
	//キーボード
	DirectXInput* input_ = DirectXInput::GetInstance();
	input_->InputIni(winApi->w, winApi->hwnd);
	//コントローラー
	Controller* controller = Controller::GetInstance();
	controller->Ini();
	//マウス
	MouseInput* mouse = MouseInput::GetInstance();
	mouse->MouseIni(winApi->hwnd);

	//ゲームシーン初期化
	GameScene* gameScene = new GameScene;
	gameScene->Ini();

	

	///////////////////////
	//描画初期化処理　ここまで//
	///////////////////////

	// ゲームループ
	while (true) {
		if (winApi->MsgCheck()) {
			break;
		}

		//インプット関連更新
		input_->InputUpdata();
		controller->Update();
		mouse->Updata();
		//ゲームシーン更新
		gameScene->Updata();
		
		//描画コマンド
		directX->PreDraw();
		//ゲームシーン描画
		gameScene->Draw();
		//描画終了
		directX->PostDraw();
	}

	// ウィンドウクラスを登録解除
	winApi->ReleaseClass();

	delete gameScene;

	

	return 0;
}
