#pragma once
#include "Object3d.h"
#include "Sprite.h"
#include "boardObject.h"
#include "WorldTransform.h"
#include "WinAPI.h"
#include "DirectX.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"


class GameScene
{
public:
	~GameScene();

	void Ini();

	void Updata();

	void Draw();
private:
	DirectXCommon* directX = nullptr;
	WinAPI* winApi_ = nullptr;
	DirectXInput* input_ = nullptr;
	TextureManager* textureM = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera;


	Object3d* gumiShip = nullptr;
	WorldTransform worldTransform_;

	Object3d* skyDome = nullptr;
	WorldTransform skyDomepos;

};

