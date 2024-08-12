#pragma once
#include "Vector2.h"
#include "Collision.h"

namespace CollisionChecker
{
    // 2次元の回転運動。回転の中心,現在位置,回転角度
    Vector2 Rotate2d(const Vector2& arg_center, const Vector2& arg_pos, float arg_radian);
    Vector2 RotateVec2(const Vector2& arg_vec2, float arg_radian);

    //-OBB--------------------------------------------------
    // 回転する矩形同士の当たり判定
    bool Check_OBB2OBB(const Square& arg_sq1, const Square& arg_sq2);
    bool Check_OBB2AABB(const Square& arg_sq1, const Square& arg_sq2);
    bool Check_OBB2Circle(const Square& arg_sq, const Circle& arg_c);

    //-AABB-------------------------------------------------
    // 矩形同士の当たり判定（回転は考慮されない）
    bool Check_AABB2AABB(const Square& arg_sq1, const Square& arg_sq2);
    // 矩形と円の当たり判定
    bool Check_AABB2Circle(const Square& arg_sq, const Circle& arg_c);
}

namespace CollisionResponse
{
    //-OBB--------------------------------------------------
    Vector2 ClosestPoint_OBB(const Square& arg_obb, const Vector2& arg_point);
    std::pair<Vector2, Vector2> Closest2Point_OBBs(const Square& arg_sq1, const Square& arg_sq2);
    Vector2 PushBack_OBB2OBB_SameRot(const Square& arg_sq1, const Square& arg_sq2);
    Vector2 PushBack_OBB2AABB(const Square& arg_sq1, const Square& arg_sq2);
    Vector2 PushBack_OBB2Circle(const Square& arg_sq, const Circle& arg_c);

    //-AABB-------------------------------------------------
    // 矩形同士の押し戻し（回転は考慮されない）
    Vector2 PushBack_AABB2AABB(const Square& arg_sq1, const Square& arg_sq2);
    // 矩形と円の押し戻し
    Vector2 PushBack_AABB2Circle(const Square& arg_sq, const Circle& arg_c);
}