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
 * @file ClearScene.h
 * @brief メインのクリアシーンクラス
 */

class ClearScene final :
	public IScene
{
private:
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;
public:
	~ClearScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;

private:
	void CameraUpdate();
};

