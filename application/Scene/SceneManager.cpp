#include "SceneManager.h"
#include "GameScene.h"
#include "WinAPI.h"
#include "Texture.h"
#include "Easing.h"
#include "ParticleScene.h"


void SceneManager::Ini()
{
	//ビルド開始時のシーン
	Transition<GameScene>();
	

	
}

void SceneManager::Update()
{
	currentScene_->Update();

	
}

void SceneManager::Draw()
{
	//ゲーム描画
	currentScene_->Draw();
}

std::unique_ptr<EmptyScene> SceneManager::currentScene_ = nullptr;

