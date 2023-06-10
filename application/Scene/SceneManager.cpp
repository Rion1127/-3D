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
	currentScene->Update();

	
}

void SceneManager::Draw()
{
	//ゲーム描画
	currentScene->Draw();
}

std::unique_ptr<EmptyScene> SceneManager::currentScene = nullptr;

Sprite SceneManager::sceneChangeSprite;
Vector2  SceneManager::sceneChangePos;
uint32_t SceneManager::sceneChangeGraph;
uint32_t SceneManager::changeStartCount_;
uint32_t SceneManager::sceneNum_;
bool SceneManager::isNext_;
uint32_t SceneManager::continueScene = SceneNum::STAGE1_;
bool SceneManager::isDifferent = true;
uint32_t SceneManager::different = 0;