#include "Gravity.h"
#include "Util.h"

/**
 * @file Gravity.cpp
 * @brief 重力の機能を持ったクラス
 */

GravityVec3::GravityVec3()
{
	addValue_ = { 0, -0.015f,0 };
	gravityVec_ = { 0,0,0 };
	gravityLimit_ = { 0,-0.5f,0 };
	isActive_ = true;
}

void GravityVec3::Update()
{
	if (isActive_) {
		gravityVec_ += addValue_;
	}
	else {
		gravityVec_ = { 0,0,0 };
	}
}

Gravity::Gravity()
{
	addValue_ = -0.015f;
	gravityValue_ = 0;
	gravityLimit_ = -0.5f;
	isActive_ = true;
}

void Gravity::Update()
{
	if (isActive_) {
		gravityValue_ += addValue_;

		gravityValue_ = Clamp(gravityValue_,-gravityLimit_, gravityLimit_);
	}
	else {
		gravityValue_ = 0;
	}
}
