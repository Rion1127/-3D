#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "IScene.h"

#include "Spline.h"
class DebugScene :
    public IScene
{
private:
	SoundManager* sound_ = nullptr;
	
	Sprite centerSprite_;
	Sprite aroundSprite_;
	float value_;
public:
	~DebugScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;
};

