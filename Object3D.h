#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Vector3.h"
class Object3D
{
private:
	Model* model;

	WorldTransform worldTrnasform_;
public:
	Object3D();
	~Object3D();

	void Init();

	void Update();
	/// <summary>
	/// ブレンド設定
	/// </summary>
	/// <param name="BLEND_ALPHA">アルファブレンド</param>
	/// <param name="BLEND_SUB">減算合成</param>
	/// <param name="BLEND_NEGA">色反転合成</param>
	/// <param name="BLEND_NORMAL">半透明合成</param>
	static void SetBlend(int blend);
	static void PreDraw();
	void Draw();

public:
	//ゲッター
	Model* GetModel() {return model;}
	WorldTransform GetWorldTransform() { return worldTrnasform_; }
	Vector3 GetPos() { return worldTrnasform_.position; }
	Vector3 GetRot() { return worldTrnasform_.rotation; }
	Vector3 GetScale() { return worldTrnasform_.scale; }
};

