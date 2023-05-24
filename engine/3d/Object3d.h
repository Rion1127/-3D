#pragma once
#include "Model.h"
#include "WorldTransform.h"

class Object3D {
private:
	Model* model_;
	WorldTransform WT_;

	//ライト
	static LightGroup* lightGroup;
public:
	Object3D();
	~Object3D();

	void Update();

	static void PreDraw();
	void Draw();

public:
	//ゲッター
	WorldTransform GetWorldTransform() { return WT_; }

public:
	static void SetLight(LightGroup* lightGroup) { Object3D::lightGroup = lightGroup; }
};