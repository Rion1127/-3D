#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"
#include "LightManager.h"

#include "Spline.h"

#include "PopEditor.h"

class EditorScene :
	public IScene
{
public:
	~EditorScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;
private:
	//>> 変数
	SoundManager* sound_ = nullptr;
	//デバッグカメラ
	DebugCamera debugCamera_;
	//ライトマネージャー
	std::shared_ptr<LightManager> lightManager_ = nullptr;

	PopEditor editor_;
};

