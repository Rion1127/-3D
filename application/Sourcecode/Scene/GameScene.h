#pragma once
#include "Sprite.h"
#include "mInput.h"
#include "Texture.h"
#include "IScene.h"
#include "mSound.h"

#include "ColliderManager.h"

#include "RectCollider.h"
/**
 * @file GameScene.h
 * @brief メインのゲームシーンクラス
 */

class GameScene final :
	public IScene
{
public:
	//>> コンストラクタ
	GameScene(void) = default;
	~GameScene(void) override = default;


	//>> 関数
	void Init()override;
	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;

private:
	//>> 変数
	SoundManager* sound_ = nullptr;
	ColliderManager colliderManager_;

	Sprite sprite_1;
	RectCollider collider_1;
	Sprite sprite_2;
	RectCollider collider_2;

private:
	void CallBack();
};

