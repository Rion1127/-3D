#pragma once
#include "Framework.h"
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
#include "LoadManager.h"
class Framework
{
private:
	//エイリアステンプレート
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	virtual ~Framework() = default;

	virtual void Init();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	//実行
	void Run();
private:
	LoadManager loadManager_;
	//std::unique_ptr<WinAPI> winApi = nullptr;
	////DirectX初期化
	//RDirectX* directX = nullptr;
	////インプット初期化
	////キーボード
	//DirectXInput* input_ = nullptr;
	////コントローラー
	//Controller* controller = nullptr;
	////マウス
	//MouseInput* mouse = nullptr;
	////サウンド初期化
	//SoundManager* sound_ = nullptr;
	////imgui初期化
	//ImGuiManager* imguiManeger_ = nullptr;
	//LoadManager
};

