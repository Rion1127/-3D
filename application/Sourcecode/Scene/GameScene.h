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
#include "PopEditor.h"
#include "FeverLight.h"
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
	const int32_t gameLimitTime = 3600;
	//>> 変数
	SoundManager* sound_ = nullptr;
	//デバッグカメラ
	DebugCamera debugCamera_;
	//ライトマネージャー
	std::shared_ptr<LightManager> lightManager_ = nullptr;

	ColliderManager colliderManager_;

	Sprite background_;
	Sprite readyText_;
	Sprite startText_;

	std::unique_ptr<Object3d> object3d_;

	Timer countdownTimer_;
	const int32_t countTime_ = 180;
	Timer startEaseTimer_;
	const int32_t startEaseTime_ = 90;
	bool isReadySEPlay;
	bool isStartSEPlay;
	Timer timer_;
	bool isTimerActive_;
	bool isTimerActiveDebug_;
	CircleGauge timerGauge_;
	CircleGauge feverGauge_;

	bool isDebug_ = false;
	bool isCanOutPut_;

	FileOutPut scoreOutPut_;
	RescueCount rescueCount_;
	PopEditor editor_;

	FeverLight feverLight_;
	std::shared_ptr<ContinuousEmitter> fireEmitter_;
	std::shared_ptr<OneceEmitter> ringCrushEmitter_;
	std::shared_ptr<OneceEmitter> ascensionEmitter_;
private:
	//ランキング更新
	void RankingUpdate();
};

