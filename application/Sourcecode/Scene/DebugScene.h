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
#include "TestDrawer.h"

#include "SphereCollider.h"

#include "KnockBack.h"
#include "Human/HumanManager.h"
#include "FeverLight.h"

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

	//std::unique_ptr<AssimpObject3D> assimpObj_;
	TestDrawer assimpObjects_;
	TestDrawer assimpObjects2_;
	Timer timer_;

	std::unique_ptr<Object3d> playerObj_;

	KnockBack knockBack_;
	Timer knockTimer_;

	Vector3 knockVec_;
	float knockPower_;

	std::unique_ptr<Object3d> colliderObj_;
	SphereCollider sphereCol_;

	FeverLight feverLight_;

	std::shared_ptr<ContinuousEmitter> fireEmitter_;
private:
	void CallBack();
};

