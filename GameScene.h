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
#include "Timer.h"

class GameScene :
	public EmptyScene
{
public:
	~GameScene();

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

	
	void CheckAllCollision();	//すべての当たり判定

	//天球
	std::unique_ptr<Object3d> skyDome;
	WorldTransform skyDomepos;
	float skyDomeRotate;

	ViewProjection gameCamera;
	void cameraUpdate();
	//使うカメラ座標
	ViewProjection* useVP = nullptr;
};

