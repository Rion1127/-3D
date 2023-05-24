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
	/// �u�����h�ݒ�
	/// </summary>
	/// <param name="BLEND_ALPHA">�A���t�@�u�����h</param>
	/// <param name="BLEND_SUB">���Z����</param>
	/// <param name="BLEND_NEGA">�F���]����</param>
	/// <param name="BLEND_NORMAL">����������</param>
	static void SetBlend(int blend);
	static void PreDraw();
	void Draw();

public:
	//�Q�b�^�[
	Model* GetModel() {return model;}
	WorldTransform GetWorldTransform() { return worldTrnasform_; }
	Vector3 GetPos() { return worldTrnasform_.position; }
	Vector3 GetRot() { return worldTrnasform_.rotation; }
	Vector3 GetScale() { return worldTrnasform_.scale; }
};

