#pragma once
#include "Sprite.h"
#include "mInput.h"
#include "Texture.h"
#include "IScene.h"
#include "mSound.h"
#include "Object3d.h"
#include "LightManager.h"
#include "DebugCamera.h"

#include "ColliderManager.h"
#include "AssimpObject3D.h"

#include "SphereCollider.h"
#include "ParticleManager.h"

class DebugScene :
    public IScene
{
public:
	~DebugScene();

	void Init()override;

	void Update()override;

	void Draw()override;
	void DrawRenderTexture()override;
	void DrawPostEffect() override;
private:
	//>> 変数
	SoundManager* sound_ = nullptr;
	//デバッグカメラ
	DebugCamera debugCamera_;
	//ライトマネージャー
	std::shared_ptr<LightManager> lightManager_ = nullptr;

	ColliderManager colliderManager_;
private:
	void CallBack();
};

