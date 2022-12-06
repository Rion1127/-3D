#include <Windows.h>
///
#include "mInput.h"
#include "Texture.h"
#include "Util.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "GameScene.h"
#include "mSound.h"
#include "SceneManager.h"
#include "PipelineManager.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include <imgui.h>
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
	input_->InputIni();
	//コントローラー
	Controller* controller = Controller::GetInstance();
	controller->Ini();
	//マウス
	MouseInput* mouse = MouseInput::GetInstance();
	mouse->MouseIni();
	//サウンド初期化
	SoundManager* sound_ = SoundManager::GetInstance();
	sound_->Init();

	PipelineManager::Ini();

	//imgui初期化
	ImGuiManager* imguiManeger_ = ImGuiManager::Getinstance();
	imguiManeger_->Init();

	//ゲームシーン初期化
	SceneManager::Ini();

	///////////////////////
	//描画初期化処理　ここまで//
	///////////////////////

	// ゲームループ
	while (true) {
		if (winApi->MsgCheck()) {
			break;
		}
		//imgui開始
		imguiManeger_->Begin();
		//インプット関連更新
		input_->InputUpdata();
		controller->Update();
		mouse->Updata();
		//デモウィンドウの表示オン
		ImGui::ShowDemoWindow();
		//ゲームシーン更新
		SceneManager::Update();
		//描画コマンド
		directX->PreDraw();
		//ゲームシーン描画
		SceneManager::Draw();
		//imgui終了
		imguiManeger_->End();
		//imgui描画
		imguiManeger_->Draw();
		//描画終了
		directX->PostDraw();
	}

	// ウィンドウクラスを登録解除
	winApi->ReleaseClass();
	//サウンド関連解放
	sound_->ReleaseAllSounds();
	//imgui解放
	imguiManeger_->Finalize();

	return 0;
}
