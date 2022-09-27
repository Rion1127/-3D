#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <d3d12.h>
#include "Object3d.h"
#include "Texture.h"
#include "math.h"
class Enemy
{
public:
	void Ini();
	void Update(ViewProjection viewprojection);

	//当たり判定を検出したら呼び出される関数
	void OnCollision();

	void Draw();
	WorldTransform GetWorldTransform() { return worldTransoform; }
	Vector3 GetPosition() { return worldTransoform.position; }
	bool GetAlive() { return isAlive; }
private:
	void Move();
	void Approach();
	void Leave();
	WorldTransform worldTransoform;
	Object3d model_{};

	uint32_t texture;

	//行動フェーズ
	enum class Phase{
		Approach,	//接近する
		Leave,		//離脱する
	};
	Phase phase_;

	const int maxHP = 10;
	int HP = maxHP;
	bool isAlive = false;

};

