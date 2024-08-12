#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"
#include "HitStop.h"
/**
 * @file Framework.ｃｐｐ
 * @brief エンジン部分の処理の流れをまとめたクラス
 */

bool Framework::isImguiDisplay_ = false;

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
	Controller::Ini();
	//マウス
	MouseInput::GetInstance()->MouseIni();
	//サウンド初期化
	SoundManager::GetInstance()->Init();
	//パイプライン初期化
	PipelineManager::Ini();
	//imgui初期化
	ImGuiManager::Getinstance()->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();
	
	loadManager_.LoadAllResources();
#ifdef _DEBUG
	Framework::isImguiDisplay_ = true;
#else
	Framework::isImguiDisplay_ = false;
#endif // _DEBUG
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
	HitStop::GetInstance()->Update();
	// ゲームループ
	//imgui開始
	ImGuiManager::Getinstance()->Begin();
	//インプット関連更新
	Key::InputUpdata();
	Controller::Update();
	MouseInput::GetInstance()->Updata();
	SoundManager::Update();

#ifdef _DEBUG
	//デモウィンドウの表示オン
	
	ImGui::Begin("ImGuiDisplay");

	ImGui::Text("他のImGuiを表示にします。このウィンドウは残ります。");
	if (ImGui::Button("ImGui"))
	{
		Framework::isImguiDisplay_ = (Framework::isImguiDisplay_ == true) ? false : true;
	}
	ImGui::SameLine();
	std::string flagName;

	if (Framework::isImguiDisplay_)flagName = "true";
	else flagName = "false";

	ImGui::Text(flagName.c_str());

	ImGui::End();
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
	//描画コマンド
	RDirectX::GetInstance()->PreDraw();
	SceneManager::Draw();
	//imgui終了
	ImGuiManager::Getinstance()->End();
	//imgui描画
	ImGuiManager::Getinstance()->Draw();
	//描画終了
	RDirectX::GetInstance()->PostDraw();
}
