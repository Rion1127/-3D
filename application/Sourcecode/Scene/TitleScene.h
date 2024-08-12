#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"

#include "Spline.h"

class TitleScene :
    public IScene
{
private:
	SoundManager* sound_ = nullptr;
	
	bool isSceneChange_;
public:
	~TitleScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;
};

