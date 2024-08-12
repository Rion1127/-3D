#pragma once
#include "Vector2.h"
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
    SHAPE_CIRCLE
};

enum SquareVertex
{
    SQUARE_LT,
    SQUARE_RT,
    SQUARE_RB,
    SQUARE_LB,

    SQUARE_MAX
};

struct MoveInfo_t
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

    //>> getter
    Vector2 Get_MoveVec(void) { return velocity.normalize(); }
};
