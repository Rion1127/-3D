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
#include "Collision.h"

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

	//テクスチャ
	uint32_t marioGraph, khGraph, enemyGraph, keyBladeGraph, gumishipGraph;
	uint32_t graph;

	//オブジェクト
	Object3d model_;
	BoardObject boardObject;
	

	Object3d* skyDome = nullptr;
	WorldTransform skyDomePos;
	Object3d* gumiShipObj = nullptr;
	WorldTransform gumiShipPos;

	//ボードオブジェクト
	WorldTransform boardPos;

	
	DebugCamera debugCamera;

	uint32_t testSound,test2Sound;
	

	ViewProjection viewProjection;

	ViewProjection useVP;

	Sphere sphere_;
	Plane plane_;

	Triangle triangle;
	
	WorldTransform trianglePos[3];
};

