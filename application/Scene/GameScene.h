#pragma once
#include "Sprite.h"
#include "DebugCamera.h"
#include "mInput.h"
#include "mSound.h"
#include "Texture.h"
#include "EmptyScene.h"
#include "LightManager.h"
#include "Object3d.h"

class GameScene final:
	public EmptyScene
{
private:
	Controller* controller_ = nullptr;
	SoundManager* sound_ = nullptr;
	DebugCamera debugCamera_;

	std::shared_ptr<LightManager> lightManager_ = nullptr;

	std::unique_ptr<Object3d> obj_;

public:
	~GameScene();

	void Ini()override;

	void Update()override;

	void Draw()override;
private:
	void CameraUpdate();
};

