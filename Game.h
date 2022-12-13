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
	//DirectX������
	DirectXCommon* directX = nullptr;
	//�C���v�b�g������
	//�L�[�{�[�h
	DirectXInput* input_ = nullptr;
	//�R���g���[���[
	Controller* controller = nullptr;
	//�}�E�X
	MouseInput* mouse = nullptr;
	//�T�E���h������
	SoundManager* sound_ = nullptr;
	//imgui������
	ImGuiManager* imguiManeger_ = nullptr;
};

