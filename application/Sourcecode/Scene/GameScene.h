#pragma once
#include "Sprite.h"
#include "mInput.h"
#include "Texture.h"
#include "IScene.h"
#include "mSound.h"
#include "Object3d.h"
#include "LightManager.h"
#include "DebugCamera.h"

#include "ColliderManager.h"

#include "RectCollider.h"
#include "Timer.h"
#include "CircleGauge.h"
#include "FileOutPut.h"
#include "RescueCount.h"
#include "ParticleManager.h"
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
	~GameScene(void) override;


	//>> 関数
	void Init()override;
	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;

private:
	SoundManager* sound_ = nullptr;
	//デバッグカメラ
	DebugCamera debugCamera_;
	//ライトマネージャー
	std::shared_ptr<LightManager> lightManager_ = nullptr;

	ColliderManager colliderManager_;

	std::unique_ptr<Object3d> object3d_;
};

