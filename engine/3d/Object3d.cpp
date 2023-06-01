#include "Object3d.h"

void Object3d::Init()
{
}

void Object3d::Update()
{
}

void Object3d::Draw(WorldTransform WT_)
{
	model_->DrawOBJ(&WT_);
}
