#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "ColliderManager.h"
#include "Spline.h"
#include "LightManager.h"

/**
 * @file ClearScene.h
 * @brief メインのクリアシーンクラス
 */

class ClearScene final :
	public IScene
{
public:
	~ClearScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;

	enum Rank {
		S = 80,
		A = 50,
		B = 20,
		C = 0,
	};
private:
	//>> 変数
	SoundManager* sound_ = nullptr;
	//デバッグカメラ
	DebugCamera debugCamera_;
	//ライトマネージャー
	std::shared_ptr<LightManager> lightManager_ = nullptr;

	bool isSceneChange_;

};

