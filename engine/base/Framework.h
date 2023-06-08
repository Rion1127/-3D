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
	//�G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	virtual ~Framework() = default;

	virtual void Init();

	virtual void Finalize();

	virtual void Update();

	virtual void Draw() = 0;

	//���s
	void Run();
private:
	LoadManager loadManager_;
	//std::unique_ptr<WinAPI> winApi = nullptr;
	////DirectX������
	//RDirectX* directX = nullptr;
	////�C���v�b�g������
	////�L�[�{�[�h
	//DirectXInput* input_ = nullptr;
	////�R���g���[���[
	//Controller* controller = nullptr;
	////�}�E�X
	//MouseInput* mouse = nullptr;
	////�T�E���h������
	//SoundManager* sound_ = nullptr;
	////imgui������
	//ImGuiManager* imguiManeger_ = nullptr;
	//LoadManager
};

