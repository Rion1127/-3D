#include "DebugScene.h"
#include "SceneManager.h"

DebugScene::~DebugScene()
{
}

void DebugScene::Init()
{
	sound_ = SoundManager::GetInstance();

	
	centerSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Box"));
	aroundSprite_.SetTexture(TextureManager::GetInstance()->GetTexture("Box"));
	
	centerSprite_.SetPos({ 700,400 });
	aroundSprite_.SetPos({ 700,400 });

	
	SoundManager::AllStop();
}

void DebugScene::Update()
{
	value_ += 0.02f;
	Vector2 pos = {
		700.f + (sinf(value_) * 100.f),
		400.f + (cosf(value_) * 100.f),
	};

	//内積の計算
	Vector2 obj2pVec = aroundSprite_.GetPos() - centerSprite_.GetPos();
	obj2pVec = obj2pVec.normalize();
	float dot = obj2pVec.dot(Vector2(1, 0));
	ImGui::Begin("debug");
	ImGui::Text("dot : %f", dot);
	ImGui::End();

	aroundSprite_.SetPos(pos);
	centerSprite_.Update();
	aroundSprite_.Update();
}

void DebugScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	
	



	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	centerSprite_.Draw();
	aroundSprite_.Draw();
}

void DebugScene::DrawRenderTexture()
{
}

void DebugScene::DrawPostEffect()
{
}
