#include "Game.h"

void Game::Init()
{
	//winAPI������
	winApi = WinAPI::GetInstance();
	winApi->Ini();

	//DirectX������
	directX = DirectXCommon::GetInstance();
	directX->Ini(winApi);

	//�e�N�X�`���}�l�[�W���[������
	TextureManager::GetInstance()->Ini(directX->GetDevice());
	//�C���v�b�g������
	//�L�[�{�[�h
	input_ = DirectXInput::GetInstance();
	input_->InputIni();
	//�R���g���[���[
	controller = Controller::GetInstance();
	controller->Ini();
	//�}�E�X
	mouse = MouseInput::GetInstance();
	mouse->MouseIni();
	//�T�E���h������
	sound_ = SoundManager::GetInstance();
	sound_->Init();

	PipelineManager::Ini();

	//imgui������
	imguiManeger_ = ImGuiManager::Getinstance();
	imguiManeger_->Init();

	//�Q�[���V�[��������
	SceneManager::Ini();
}

void Game::Run()
{
	// �Q�[�����[�v
	while (true) {
		if (winApi->MsgCheck()) {
			break;
		}
		//imgui�J�n
		imguiManeger_->Begin();
		//�C���v�b�g�֘A�X�V
		input_->InputUpdata();
		controller->Update();
		mouse->Updata();
		//�f���E�B���h�E�̕\���I��
		ImGui::ShowDemoWindow();
		//�Q�[���V�[���X�V
		SceneManager::Update();
		//�`��R�}���h
		directX->PreDraw();
		//�Q�[���V�[���`��
		SceneManager::Draw();
		//imgui�I��
		imguiManeger_->End();
		//imgui�`��
		imguiManeger_->Draw();
		//�`��I��
		directX->PostDraw();
	}
	//���
	Release();
}

void Game::Release()
{
	// �E�B���h�E�N���X��o�^����
	winApi->ReleaseClass();
	//�T�E���h�֘A���
	sound_->ReleaseAllSounds();
	//imgui���
	imguiManeger_->Finalize();
}

