#pragma once
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
class Game
{
public:

	void Init();

	void Run();

private:
	void Release();

	WinAPI* winApi = nullptr;
	//DirectX初期化
	DirectXCommon* directX = nullptr;
	//インプット初期化
	//キーボード
	DirectXInput* input_ = nullptr;
	//コントローラー
	Controller* controller = nullptr;
	//マウス
	MouseInput* mouse = nullptr;
	//サウンド初期化
	SoundManager* sound_ = nullptr;
	//imgui初期化
	ImGuiManager* imguiManeger_ = nullptr;
};

