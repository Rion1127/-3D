#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

void Framework::Init()
{
	//winAPI初期化
	WinAPI::GetInstance()->Ini();

	//DirectX初期化
	RDirectX::GetInstance()->Ini(WinAPI::GetInstance());

	//テクスチャマネージャー初期化
	TextureManager::GetInstance()->Ini();
	//インプット初期化
	//キーボード
	Key::InputIni();
	//コントローラー
	Controller::GetInstance()->Ini();
	//マウス
	MouseInput::GetInstance()->MouseIni();
	//サウンド初期化
	SoundManager::GetInstance()->Init();

	PipelineManager::Ini();

	//imgui初期化
	ImGuiManager::Getinstance()->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();

	loadManager_.LoadAllResources();

	bloom_ = std::move(std::make_unique<Bloom>());
	noise_ = std::move(std::make_unique<Noise>());
}

void Framework::Finalize()
{
	// ウィンドウクラスを登録解除
	WinAPI::GetInstance()->ReleaseClass();
	//サウンド関連解放
	SoundManager::GetInstance()->ReleaseAllSounds();
	//imgui解放
	ImGuiManager::Getinstance()->Finalize();

	
}

void Framework::Update()
{
	// ゲームループ

	//imgui開始
	ImGuiManager::Getinstance()->Begin();
	//インプット関連更新
	Key::InputUpdata();
	Controller::GetInstance()->Update();
	MouseInput::GetInstance()->Updata();

	//noise_->PUpdate();
#ifdef _DEBUG
	//デモウィンドウの表示オン
	//ImGui::ShowDemoWindow();
#endif // DEBUG
}

void Framework::Run()
{
	//初期化
	Init();

	while (true) {
		if (WinAPI::GetInstance()->MsgCheck()) {
			break;
		}

		//毎フレーム処理
		Update();
		
		//描画
		Draw();
	}
	//ゲームの終了
	Finalize();
}

void Framework::Draw()
{
	//レンダーテクスチャへの描画
	//noise_->PreDrawScene();
	//SceneManager::Draw();
	//noise_->PostDrawScene();

	//bloom_->PreDraw();

	//描画コマンド
	RDirectX::GetInstance()->PreDraw();
	//ゲームシーン描画
	SceneManager::Draw();
	//bloom_->Draw();
	//noise_->Draw("Noise");
	//imgui終了
	ImGuiManager::Getinstance()->End();
	//imgui描画
	ImGuiManager::Getinstance()->Draw();
	//描画終了
	RDirectX::GetInstance()->PostDraw();
}
