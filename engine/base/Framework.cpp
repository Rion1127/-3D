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
	DirectXInput::GetInstance()->InputIni();
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

	postEffect = std::move(std::make_unique<PostEffect>());
	postEffect->SetTexture(*TextureManager::GetInstance()->GetTexture("White"));
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
	DirectXInput::GetInstance()->InputUpdata();
	Controller::GetInstance()->Update();
	MouseInput::GetInstance()->Updata();

	postEffect->PUpdate();
#ifdef _DEBUG
	//�f���E�B���h�E�̕\���I��
	ImGui::ShowDemoWindow();
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
	postEffect->PreDrawScene();
	SceneManager::Draw();
	postEffect->PostDrawScene();

	//�`��R�}���h
	RDirectX::GetInstance()->PreDraw();
	//�Q�[���V�[���`��
	postEffect->Draw();
	//imgui�I��
	ImGuiManager::Getinstance()->End();
	//imgui�`��
	ImGuiManager::Getinstance()->Draw();
	//�`��I��
	RDirectX::GetInstance()->PostDraw();
}
