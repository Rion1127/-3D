#pragma once
#include <memory>
#include "Object3d.h"
#include "Sprite.h"
#include "boardObject.h"
#include "WorldTransform.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "EmptyScene.h"
#include <sstream>
#include <fstream>

class ParticleScene :
	public EmptyScene
{
public:
	~ParticleScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	DirectXCommon* directX = nullptr;
	WinAPI* winApi_ = nullptr;
	DirectXInput* input_ = nullptr;
	Controller* controller_ = nullptr;
	TextureManager* textureM = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera;

	ViewProjection gameCamera;
	//égÇ§ÉJÉÅÉâç¿ïW
	ViewProjection* useVP = nullptr;

	BoardObject object_;
	WorldTransform WT_;

	uint32_t texture_;
	uint32_t whiteTexture_;
};