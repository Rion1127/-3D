#include "Framework.h"
#include "DirectionalLight.h"
#include "LightGroup.h"

void Framework::Init()
{
	//winAPI������
	WinAPI::GetInstance()->Ini();

	//DirectX������
	RDirectX::GetInstance()->Ini(WinAPI::GetInstance());

	//�e�N�X�`���}�l�[�W���[������
	TextureManager::GetInstance()->Ini();
	//�C���v�b�g������
	//�L�[�{�[�h
	Key::InputIni();
	//�R���g���[���[
	Controller::GetInstance()->Ini();
	//�}�E�X
	MouseInput::GetInstance()->MouseIni();
	//�T�E���h������
	SoundManager::GetInstance()->Init();

	PipelineManager::Ini();

	//imgui������
	ImGuiManager::Getinstance()->Init();

	DirectionalLight::StaticInit();
	LightGroup::StaticInit();

	loadManager_.LoadAllResources();

	bloom_ = std::move(std::make_unique<Bloom>());
	noise_ = std::move(std::make_unique<Noise>());
}

void Framework::Finalize()
{
	// �E�B���h�E�N���X��o�^����
	WinAPI::GetInstance()->ReleaseClass();
	//�T�E���h�֘A���
	SoundManager::GetInstance()->ReleaseAllSounds();
	//imgui���
	ImGuiManager::Getinstance()->Finalize();

	
}

void Framework::Update()
{
	// �Q�[�����[�v

	//imgui�J�n
	ImGuiManager::Getinstance()->Begin();
	//�C���v�b�g�֘A�X�V
	Key::InputUpdata();
	Controller::GetInstance()->Update();
	MouseInput::GetInstance()->Updata();

	//noise_->PUpdate();
#ifdef _DEBUG
	//�f���E�B���h�E�̕\���I��
	//ImGui::ShowDemoWindow();
#endif // DEBUG
}

void Framework::Run()
{
	//������
	Init();

	while (true) {
		if (WinAPI::GetInstance()->MsgCheck()) {
			break;
		}

		//���t���[������
		Update();
		
		//�`��
		Draw();
	}
	//�Q�[���̏I��
	Finalize();
}

void Framework::Draw()
{
	//�����_�[�e�N�X�`���ւ̕`��
	//noise_->PreDrawScene();
	//SceneManager::Draw();
	//noise_->PostDrawScene();

	//bloom_->PreDraw();

	//�`��R�}���h
	RDirectX::GetInstance()->PreDraw();
	//�Q�[���V�[���`��
	SceneManager::Draw();
	//bloom_->Draw();
	//noise_->Draw("Noise");
	//imgui�I��
	ImGuiManager::Getinstance()->End();
	//imgui�`��
	ImGuiManager::Getinstance()->Draw();
	//�`��I��
	RDirectX::GetInstance()->PostDraw();
}
