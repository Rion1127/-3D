#pragma once
#include "Sprite.h"
#include "mInput.h"
#include "Texture.h"
#include "IScene.h"
#include "mSound.h"

#include "ColliderManager.h"

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
};

