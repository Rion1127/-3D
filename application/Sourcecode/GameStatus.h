#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "cstdint"

enum Direction
{
	DIR_RIGHT,
	DIR_LEFT,
	DIR_TOP,
	DIR_BOTTOM,
};

enum Shape
{
	SHAPE_INTERFACE,
	SHAPE_RECT,
	SHAPE_CIRCLE,
	SHAPE_3D,//以降3Dの当たり判定
	SHAPE_SPHERE,
};

enum SquareVertex
{
	SQUARE_LT,
	SQUARE_RT,
	SQUARE_RB,
	SQUARE_LB,

	SQUARE_MAX
};

struct MoveInfo2d_t
{
	//>> 関数
	void Initialize(void)
	{
		velocity = { 0,0 };
	}
	
	//>> 変数
	Vector2 velocity;
	//徐々に減衰するベクトル
	Vector2 attenVelocity;
	//減衰速度
	float attenValue = 0.1f;

	//>> getter
	Vector2 Get_MoveVec(void) { return velocity.normalize(); }
};

struct MoveInfo3d_t
{
	//>> 関数
	void Initialize(void)
	{
		velocity = { 0,0,0 };
	}

	//>> 変数
	Vector3 velocity;
	//徐々に減衰するベクトル
	Vector3 attenVelocity;
	//減衰速度
	float attenValue = 0.1f;

	//>> getter
	Vector3 Get_MoveVec(void) { return velocity.normalize(); }
};

enum TestState {
	IDLE,
	WALK,
	RUN
};
