#include "TitleScene.h"
#include "SceneManager.h"
#include "GameScene.h"
TitleScene::~TitleScene()
{
}

void TitleScene::Ini()
{
	controller_ = Controller::GetInstance();
	sound_ = SoundManager::GetInstance();

	pressASprite_ = std::move(std::make_unique<Sprite>());
	pressASprite_->Ini();
	pressASprite_->SetTexture(TextureManager::GetInstance()->GetTexture("PressA"));
	Vector2 pos = {
		WinAPI::GetWindowSize().x / 2.f,
		WinAPI::GetWindowSize().y / 1.2f
	};
	pressASprite_->SetPos(pos);
}

void TitleScene::Update()
{
	CameraUpdate();
	
	if (Controller::GetInstance()->GetTriggerButtons(PAD::INPUT_A) ||
		Key::TriggerKey(DIK_SPACE)) {
		SceneManager::SetChangeStart(SceneName::Game);
	}

	pressASprite_->Update();
}

void TitleScene::Draw()
{
	PipelineManager::PreDraw("Sprite");
	pressASprite_->Draw();
	
	PipelineManager::PreDraw("Object3D");

	PipelineManager::PreDraw("Toon");

	PipelineManager::PreDraw("assimp");

	
	PipelineManager::PreDraw("Sprite");
	

	PipelineManager::PreDraw("Particle",TopologyName::POINTLIST);
	
}

void TitleScene::CameraUpdate()
{
	
	Camera::scurrent_ = debugCamera_.GetCamera();

	Camera::scurrent_->Update(CameraMode::LookAT);
}
