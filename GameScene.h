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
#include "LightGroup.h"

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
	
	void CheckAllCollision();	//���ׂĂ̓����蔻��

	ViewProjection gameCamera;
	void cameraUpdate();
	//�g���J�������W
	ViewProjection* useVP = nullptr;

	LightGroup* lightGroup = nullptr;

	float t = 0;

	std::unique_ptr<Object3d> skyDome_;
	WorldTransform worldTransform_;

	std::unique_ptr<Object3d> sphere_;
	WorldTransform sphereWT_;

	std::unique_ptr<Object3d> floor_;
	WorldTransform floorWT_;

	float ambientColor0[3] = { 0,0,0 };

	//���������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	//�_����
	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f,0.1f,0.1f };
};

