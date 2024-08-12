#include "GameScene.h"
#include "Easing.h"
#include "SceneManager.h"
#include "Collision.h"
#include "Camera.h"
#include "JsonLoader.h"
#include "Framework.h"
#include "Camera2d.h"
#include "ClearScene.h"
#include <filesystem>
#include "mSound.h"
#include "Framework.h"

/**
 * @file GameScene.cpp
 * @brief メインのゲームシーンクラス
 */

void GameScene::Init()
{
	Camera2d::GetInstance()->Init();

	sprite_1.SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	sprite_2.SetTexture(TextureManager::GetInstance()->GetTexture("White"));
	
	sprite_1.SetPos(Vector2(600, 300));
	sprite_2.SetPos(Vector2(400, 300));
	sprite_1.SetScale(Vector2(64, 64));
	sprite_2.SetScale(Vector2(64,64));

	collider_1.square_.center = sprite_1.GetPos();
	collider_1.square_.length = sprite_1.GetScale();
	collider_2.square_.center = sprite_2.GetPos();
	collider_2.square_.length = sprite_2.GetScale();
	//ハンマーが当たった時のプレイヤーの関数を格納する
	auto callback_1 = std::bind(&GameScene::CallBack, this);
	collider_1.Initialize("rect_1", callback_1, &colliderManager_);
	auto callback_2 = std::bind(&GameScene::CallBack, this);
	collider_2.Initialize("rect_2", callback_2, &colliderManager_);
}

void GameScene::Update()
{
#ifdef _DEBUG
	
#endif // _DEBUG
	Vector2 pos = sprite_1.GetPos();

	pos.x += 2.f * (Key::PushKey(DIK_D) - Key::PushKey(DIK_A));
	pos.y += 2.f * (Key::PushKey(DIK_S) - Key::PushKey(DIK_W));

	sprite_1.SetPos(pos);

	collider_1.square_.center = sprite_1.GetPos();
	collider_1.square_.length = sprite_1.GetScale();
	collider_2.square_.center = sprite_2.GetPos();
	collider_2.square_.length = sprite_2.GetScale();
	
	colliderManager_.Update();

	sprite_1.Update();
	sprite_2.Update();
}

void GameScene::Draw()
{
	/////////////////
	//バックスプライト//
	/////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
	sprite_1.Draw();
	sprite_2.Draw();
	///////////////////
	//フロントスプライト//
	//////////////////
	PipelineManager::PreDraw("Sprite", TRIANGLELIST);
}

void GameScene::DrawRenderTexture()
{
}

void GameScene::DrawPostEffect()
{
	PipelineManager::PreDraw("Particle", POINTLIST);
}

void GameScene::CallBack()
{
	if (collider_1.IsDetect_Name("rect_2")) {
		sprite_1.SetColor(Color(255, 0, 0, 255));
	}
	if (collider_1.IsRelease_Name("rect_2")) {
		sprite_1.SetColor(Color(255, 255, 255, 255));
	}
}
