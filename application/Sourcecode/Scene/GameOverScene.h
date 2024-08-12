#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "ColliderManager.h"
#include "Spline.h"

/**
 * @file GameOverScene.h
 * @brief メインのクリアシーンクラス
 */

class GameOverScene final :
	public IScene
{
private:
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;
	bool isSceneChange_;
public:
	~GameOverScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;

private:
	void CameraUpdate();
};

