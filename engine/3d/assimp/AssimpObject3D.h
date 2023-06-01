#pragma once
#include "AssinpModel.h"
#include "WorldTransform.h"
class AssimpObject3D
{
public:
	void SetModel(AssinpModel* model) { model_ = model; }

	void Update();

	void Draw();

private:
	AssinpModel* model_ = nullptr;
	WorldTransform worldTransform_;
};

