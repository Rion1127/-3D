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

	//�e�N�X�`��
	uint32_t marioGraph, khGraph, enemyGraph, keyBladeGraph, gumishipGraph;
	uint32_t graph;

	//�I�u�W�F�N�g
	Object3d model_;
	BoardObject boardObject;
	Sprite Obj2D;

	Object3d* skyDome = nullptr;
	Object3d* gumiShipObj = nullptr;
	WorldTransform gumiShipPos;

	//�{�[�h�I�u�W�F�N�g
	WorldTransform boardPos;

	//3D�I�u�W�F�N�g�̐�
	static const size_t kObjectCount = 20;
	//3D�I�u�W�F�N�g�̔z��
	WorldTransform worldTransform[kObjectCount];
	//�I�u�W�F�N�g�F�ύX
	bool isChangeColor = false;
	XMFLOAT4 objectColor{ 1,1,1,1 };

	DebugCamera debugCamera;

	uint32_t testSound,test2Sound;
	//�X�v���C�g�̍��W
	Vector2 spritePos = { 100,200 };

	ViewProjection viewProjection;

	ViewProjection useVP;
};

