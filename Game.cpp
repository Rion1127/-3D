#include "Game.h"

void Game::Init()
{
	//winAPI初期化
	winApi = WinAPI::GetInstance();
	winApi->Ini();

	//DirectX初期化
	directX = DirectXCommon::GetInstance();
	directX->Ini(winApi);

	//テクスチャマネージャー初期化
	TextureManager::GetInstance()->Ini(directX->GetDevice());
	//インプット初期化
	//キーボード
	input_ = DirectXInput::GetInstance();
	input_->InputIni();
	//コントローラー
	controller = Controller::GetInstance();
	controller->Ini();
	//マウス
	mouse = MouseInput::GetInstance();
	mouse->MouseIni();
	//サウンド初期化
	sound_ = SoundManager::GetInstance();
	sound_->Init();

	PipelineManager::Ini();

	//imgui初期化
	imguiManeger_ = ImGuiManager::Getinstance();
	imguiManeger_->Init();

	//ゲームシーン初期化
	SceneManager::Ini();
}

void Game::Run()
{
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
	//解放
	Release();
}

void Game::Release()
{
	// ウィンドウクラスを登録解除
	winApi->ReleaseClass();
	//サウンド関連解放
	sound_->ReleaseAllSounds();
	//imgui解放
	imguiManeger_->Finalize();
}

