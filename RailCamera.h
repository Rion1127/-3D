#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class RailCamera
{
public:
	void Ini(Vector3 pos, Vector3 rot);

	void Update();

	//ビュープロジェクション
	ViewProjection viewProjection;
private:
	//ワールド変換データ
	WorldTransform worldTransform;

};

