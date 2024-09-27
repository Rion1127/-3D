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

	Sprite resultSprite_;	//「結果」
	Sprite backGround_;	//背景
	Sprite uiSprite_;	//Aボタン
	Sprite rankItemSprite_;	//「腕前」
	Sprite rankTextSprite_;	//～～級テキスト
	Sprite retryTextSprite_;	//再挑戦テキスト
	Sprite titleTextSprite_;	//戻るテキスト
	bool isSceneChange_;

	Vector2 retryPos;
	Vector2 titlePos;
	TimerFloat uiEasingTimer_;

	TimerFloat uiFloatTimer_;
	int32_t uiAnimationIndex_;

	TimerFloat scoreEasing_;

	bool isSEPray;	//SE鳴らしたか

	bool isRetry;	//リトライかタイトルか
private:
	void LoadRanking();

	void RankSpriteInit();
	//階級
	void Rank();
};

