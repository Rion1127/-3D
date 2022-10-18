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
	mSound* sound_ = nullptr;

	//テクスチャ
	uint32_t marioGraph, khGraph, enemyGraph, keyBladeGraph, gumishipGraph;
	uint32_t graph;

	//オブジェクト
	Object3d model_;
	BoardObject boardObject;
	Object2D Obj2D;

	Object3d* skyDome = nullptr;
	Object3d* gumiShipObj = nullptr;
	WorldTransform gumiShipPos;

	//ボードオブジェクト
	WorldTransform boardPos;

	//3Dオブジェクトの数
	static const size_t kObjectCount = 20;
	//3Dオブジェクトの配列
	WorldTransform worldTransform[kObjectCount];
	//オブジェクト色変更
	bool isChangeColor = false;
	XMFLOAT4 objectColor{ 1,1,1,1 };

	DebugCamera debugCamera;

	uint32_t testSound;
};

