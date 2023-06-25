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
	gaussianBlur_ = std::move(std::make_unique<GaussianBlur>());
	crossFilter_ = std::move(std::make_unique<CrossFilter>());
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

	const char* postEffectName = "None";

	if (postEffectnum >= (size_t)PostEffectName::END)postEffectnum = 0;

	if (PostEffectName::Gaussian == PostEffectName(postEffectnum)) {
		gaussianBlur_->PUpdate();
		postEffectName = "Gaussian";
	}
	else if (PostEffectName::Bloom == PostEffectName(postEffectnum)) {
		bloom_->Update();
		postEffectName = "Bloom";
	}
	else if (PostEffectName::Noise == PostEffectName(postEffectnum)) {
		noise_->PUpdate();
		postEffectName = "Noise";
	}
	else if (PostEffectName::CrossFilter == PostEffectName(postEffectnum)) {
		crossFilter_->Update();
		postEffectName = "Crossfilter";
	}

	ImGui::Begin("postEffect");
	if (ImGui::Button("Change PostEffect"))postEffectnum++;
	ImGui::Text(postEffectName);
	ImGui::End();
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
	if (PostEffectName::Gaussian == PostEffectName(postEffectnum)) {
		gaussianBlur_->PreDraw();
	}
	else if (PostEffectName::Bloom == PostEffectName(postEffectnum)) {
		bloom_->PreDraw();
	}
	else if (PostEffectName::Noise == PostEffectName(postEffectnum)) {
		noise_->PreDraw();
	}
	else if (PostEffectName::CrossFilter == PostEffectName(postEffectnum)) {
		crossFilter_->PreDraw();
	}

	//�`��R�}���h
	RDirectX::GetInstance()->PreDraw();
	//�Q�[���V�[���`��
	if (PostEffectName::None == PostEffectName(postEffectnum)) {
		SceneManager::Draw();
	}
	else if (PostEffectName::Gaussian == PostEffectName(postEffectnum)) {
		gaussianBlur_->Draw("Gaussian");
	}
	else if (PostEffectName::Bloom == PostEffectName(postEffectnum)) {
		bloom_->Draw();
	}
	else if (PostEffectName::Noise == PostEffectName(postEffectnum)) {
		noise_->Draw("Noise");
	}
	else if (PostEffectName::CrossFilter == PostEffectName(postEffectnum)) {
		crossFilter_->Draw();
	}
	//imgui�I��
	ImGuiManager::Getinstance()->End();
	//imgui�`��
	ImGuiManager::Getinstance()->Draw();
	//�`��I��
	RDirectX::GetInstance()->PostDraw();
}
