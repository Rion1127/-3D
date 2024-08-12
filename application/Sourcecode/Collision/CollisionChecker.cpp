#include "CollisionChecker.h"
#include "GameStatus.h"
#include <array>
#include <algorithm>

Vector2 CollisionChecker::Rotate2d(const Vector2& arg_center, const Vector2& arg_pos, float arg_radian)
{
    const Vector2& diff = Vector2(arg_pos - arg_center);
    Vector2 result = RotateVec2(diff, arg_radian);

    return result + arg_center;
}

Vector2 CollisionChecker::RotateVec2(const Vector2& arg_vec2, float arg_radian)
{
    Vector2 result;
    result.x = arg_vec2.x * std::cosf(arg_radian) - arg_vec2.y * std::sinf(arg_radian);
    result.y = arg_vec2.x * std::sinf(arg_radian) + arg_vec2.y * std::cosf(arg_radian);

    return result;
}

bool CollisionChecker::Check_OBB2OBB(const Square& arg_sq1, const Square& arg_sq2)
{
    //-OBB1----------------------------------------------------
    // 半径
    const Vector2 harfL1 = arg_sq1.length / 2;
    // 頂点|basic
    Vector2 vertices1[4]; // 左上,右上,右下,左下
    vertices1[SQUARE_LT] = Vector2(arg_sq1.center - harfL1);
    vertices1[SQUARE_RT] = Vector2(arg_sq1.center.x + harfL1.x, arg_sq1.center.y - harfL1.y);
    vertices1[SQUARE_RB] = Vector2(arg_sq1.center + harfL1);
    vertices1[SQUARE_LB] = Vector2(arg_sq1.center.x - harfL1.x, arg_sq1.center.y + harfL1.y);
    // 頂点|rotate
    vertices1[SQUARE_LT] = Rotate2d(arg_sq1.center, vertices1[SQUARE_LT], arg_sq1.radian);
    vertices1[SQUARE_RT] = Rotate2d(arg_sq1.center, vertices1[SQUARE_RT], arg_sq1.radian);
    vertices1[SQUARE_RB] = Rotate2d(arg_sq1.center, vertices1[SQUARE_RB], arg_sq1.radian);
    vertices1[SQUARE_LB] = Rotate2d(arg_sq1.center, vertices1[SQUARE_LB], arg_sq1.radian);

    //-OBB2----------------------------------------------------
    // 半径
    const Vector2 harfL2 = arg_sq2.length / 2;
    // 頂点|basic
    Vector2 vertices2[4]; // 左上,右上,右下,左下
    vertices2[SQUARE_LT] = Vector2(arg_sq2.center - harfL2);
    vertices2[SQUARE_RT] = Vector2(arg_sq2.center.x + harfL2.x, arg_sq2.center.y - harfL2.y);
    vertices2[SQUARE_RB] = Vector2(arg_sq2.center + harfL2);
    vertices2[SQUARE_LB] = Vector2(arg_sq2.center.x - harfL2.x, arg_sq2.center.y + harfL2.y);
    // 頂点|rotate
    vertices2[SQUARE_LT] = Rotate2d(arg_sq2.center, vertices2[SQUARE_LT], arg_sq2.radian);
    vertices2[SQUARE_RT] = Rotate2d(arg_sq2.center, vertices2[SQUARE_RT], arg_sq2.radian);
    vertices2[SQUARE_RB] = Rotate2d(arg_sq2.center, vertices2[SQUARE_RB], arg_sq2.radian);
    vertices2[SQUARE_LB] = Rotate2d(arg_sq2.center, vertices2[SQUARE_LB], arg_sq2.radian);

    // 分離直線
    Vector2 sideDirs[4];
    sideDirs[0] = Vector2(vertices1[SQUARE_LT] - vertices1[SQUARE_RT]).normalize(); // 右上->左上
    sideDirs[1] = Vector2(vertices1[SQUARE_RT] - vertices1[SQUARE_RB]).normalize(); // 右下->右上
    sideDirs[2] = Vector2(vertices2[SQUARE_LT] - vertices2[SQUARE_RT]).normalize(); // 右上->左上
    sideDirs[3] = Vector2(vertices2[SQUARE_RT] - vertices2[SQUARE_RB]).normalize(); // 右下->右上

    for (const auto& u : sideDirs)
    {
        float min1 = vertices1[0].dot(u);
        float max1 = min1;
        float min2 = vertices2[0].dot(u);
        float max2 = min2;

        for (int i = 1; i < 4; i++)
        {
            //-BOX1-------------------------------------
            // 射影距離の算出
            float proj1 = vertices1[i].dot(u);
            // 最小/最大の場合、更新する
            if (proj1 < min1) { min1 = proj1; }
            else if (proj1 > max1) { max1 = proj1; }

            //-BOX2-------------------------------------
            // 射影距離の算出
            float proj2 = vertices2[i].dot(u);
            // 最小/最大の場合、更新する
            if (proj2 < min2) { min2 = proj2; }
            else if (proj2 > max2) { max2 = proj2; }
        }

        // 衝突判定: 触れていない事を証明する
        if (max2 < min1 || max1 < min2)
        {
            return false;
        }
    }

    // 衝突している
    return true;
}

bool CollisionChecker::Check_OBB2AABB(const Square& arg_sq1, const Square& arg_sq2)
{
    //-OBB------------------------------------------
    const float rad = arg_sq1.radian;
    const float sin = std::sin(rad);
    const float cos = std::cos(rad);
    Matrix4 obb_matRotZ
    {
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    // ワールド行列宣言
    Matrix4 obb_matWorld{};
    // ワールド行列初期化
    obb_matWorld.UnitMatrix();

    // 回転行列の合成
    obb_matWorld *= obb_matRotZ;
    // 移動行列の合成
    obb_matWorld.m[3][0] = arg_sq1.center.x;
    obb_matWorld.m[3][1] = arg_sq1.center.y;

    //-AABB----------------------------------------
    // ワールド行列宣言
    Matrix4 aabb_matWorld{};
    // ワールド行列初期化
    aabb_matWorld.UnitMatrix();

    // 移動行列の合成
    aabb_matWorld.m[3][0] = arg_sq2.center.x;
    aabb_matWorld.m[3][1] = arg_sq2.center.y;

    //-計算処理----------------------------------------
    // 逆行列の生成
    Matrix4 obb_inv = obb_matWorld.Inverse();
    obb_matWorld *= obb_inv;        // 回転の打ち消し
    aabb_matWorld *= obb_inv;       // ローカル座標化

    // AABBに変換
    Square aabb1(Vector2{ obb_matWorld.m[3][0], obb_matWorld.m[3][1] }, arg_sq1.length);
    Square aabb2(Vector2{ aabb_matWorld.m[3][0], aabb_matWorld.m[3][1] }, arg_sq2.length);

    return CollisionChecker::Check_AABB2AABB(aabb1, aabb2);
}

bool CollisionChecker::Check_OBB2Circle(const Square& arg_sq, const Circle& arg_c)
{
    //-OBB------------------------------------------
    const float rad = arg_sq.radian;
    const float sin = std::sin(rad);
    const float cos = std::cos(rad);
    Matrix4 obb_matRotZ
    {
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    // ワールド行列宣言
    Matrix4 obb_matWorld{};
    // ワールド行列初期化
    obb_matWorld.UnitMatrix();

    // 回転行列の合成
    obb_matWorld *= obb_matRotZ;
    // 移動行列の合成
    obb_matWorld.m[3][0] = arg_sq.center.x;
    obb_matWorld.m[3][1] = arg_sq.center.y;

    //-Circle---------------------------------------
    // ワールド行列宣言
    Matrix4 circle_matWorld{};
    // ワールド行列初期化
    circle_matWorld.UnitMatrix();

    // 移動行列の合成
    circle_matWorld.m[3][0] = arg_c.center.x;
    circle_matWorld.m[3][1] = arg_c.center.y;

    //-計算処理----------------------------------------
    // 逆行列の生成
    Matrix4 mWorld1Inv = obb_matWorld.Inverse();
    obb_matWorld *= mWorld1Inv;         // 回転の打ち消し
    circle_matWorld *= mWorld1Inv;      // ローカル座標化

    // AABBに変換
    Square aabb(Vector2{ obb_matWorld.m[3][0], obb_matWorld.m[3][1] }, arg_sq.length);
    Circle circle(Vector2{ circle_matWorld.m[3][0], circle_matWorld.m[3][1] }, arg_c.radius);

    return CollisionChecker::Check_AABB2Circle(aabb, circle);
}

bool CollisionChecker::Check_AABB2AABB(const Square& arg_sq1, const Square& arg_sq2)
{
    //-AABB1-------------------
    Vector2 harf1 = arg_sq1.length / 2;
    Vector2 sq1_min, sq1_max;
    sq1_min = arg_sq1.center - harf1;
    sq1_max = arg_sq1.center + harf1;

    //-AABB2-------------------
    Vector2 harf2 = arg_sq2.length / 2;
    Vector2 sq2_min, sq2_max;
    sq2_min = arg_sq2.center - harf2;
    sq2_max = arg_sq2.center + harf2;

    // X軸: AABB同士触れていない => 接触していない
    if (sq1_max.x < sq2_min.x || sq2_max.x < sq1_min.x) { return false; }
    // Y軸: AABB同士触れていない => 接触していない
    if (sq1_max.y < sq2_min.y || sq2_max.y < sq1_min.y) { return false; }

    // 接触している
    return true;
}

bool CollisionChecker::Check_AABB2Circle(const Square& arg_sq, const Circle& arg_c)
{
    //-AABB-------------------
    Vector2 harf = arg_sq.length / 2;
    Vector2 sq_min{}, sq_max{};
    sq_min = arg_sq.center - harf;
    sq_max = arg_sq.center + harf;

    // AABB側の最近接点
    Vector2 closestPoint{};
    closestPoint.x = Clamp(arg_c.center.x, sq_min.x, sq_max.x);
    closestPoint.y = Clamp(arg_c.center.y, sq_min.y, sq_max.y);
    // 最近接点=>円の中心までの距離
    float dist = Vector2(arg_c.center - closestPoint).length();

    // 距離が円半径よりちいさいなら衝突している
    if (dist < arg_c.radius) { return true; }
    return false;
}

Vector2 CollisionResponse::ClosestPoint_OBB(const Square& arg_obb, const Vector2& arg_point)
{
    // 点からobbの中心へのベクトル
    const Vector2 d = arg_point - arg_obb.center;
    // 最近接点
    Vector2 closest = arg_obb.center;

    // 定数: 軸数
    constexpr size_t kAxesNum = 2;
    // obbの回転を表す軸
    std::array<Vector2, kAxesNum> axes = { Vector2(1, 0), Vector2(0, -1) };

    // 軸ごとに計算
    for (size_t i = 0; i < kAxesNum; ++i) {
        // 軸をobbの持つ回転量だけ、初期値から回転
        axes[i] = CollisionChecker::RotateVec2(axes[i], arg_obb.radian).normalize();
        // 中心点からの距離
        float distance = d.dot(axes[i]);
        // 大きさの半分
        float harfLength = (i == 0) ? arg_obb.length.x / 2 : arg_obb.length.y / 2;

        // 距離の最大/最小範囲をobbの矩形に合わせる
        if (distance > harfLength) { distance = harfLength; }
        if (distance < -harfLength) { distance = -harfLength; }

        // 最近接点に代入: obbの中心点 + 方向 * 距離
        closest = closest + axes[i] * distance;
    }

    return closest;
}

std::pair<Vector2, Vector2> CollisionResponse::Closest2Point_OBBs(const Square& arg_sq1, const Square& arg_sq2)
{
    using namespace CollisionChecker;

    // 
    Vector2 closestPoint1, closestPoint2;
    float minDist = FLT_MAX;

    //-OBB1----------------------------------------------------
    // 半径
    const Vector2 harfL1 = arg_sq1.length / 2;
    // 頂点|basic
    std::array<Vector2, 4> vertices1; // 左上,右上,右下,左下
    vertices1[SQUARE_LT] = Vector2(arg_sq1.center - harfL1);
    vertices1[SQUARE_RT] = Vector2(arg_sq1.center.x + harfL1.x, arg_sq1.center.y - harfL1.y);
    vertices1[SQUARE_RB] = Vector2(arg_sq1.center + harfL1);
    vertices1[SQUARE_LB] = Vector2(arg_sq1.center.x - harfL1.x, arg_sq1.center.y + harfL1.y);
    // 頂点|rotate
    vertices1[SQUARE_LT] = Rotate2d(arg_sq1.center, vertices1[SQUARE_LT], arg_sq1.radian);
    vertices1[SQUARE_RT] = Rotate2d(arg_sq1.center, vertices1[SQUARE_RT], arg_sq1.radian);
    vertices1[SQUARE_RB] = Rotate2d(arg_sq1.center, vertices1[SQUARE_RB], arg_sq1.radian);
    vertices1[SQUARE_LB] = Rotate2d(arg_sq1.center, vertices1[SQUARE_LB], arg_sq1.radian);

    //-OBB2----------------------------------------------------
    // 半径
    const Vector2 harfL2 = arg_sq2.length / 2;
    // 頂点|basic
    std::array<Vector2, 4> vertices2; // 左上,右上,右下,左下
    vertices2[SQUARE_LT] = Vector2(arg_sq2.center - harfL2);
    vertices2[SQUARE_RT] = Vector2(arg_sq2.center.x + harfL2.x, arg_sq2.center.y - harfL2.y);
    vertices2[SQUARE_RB] = Vector2(arg_sq2.center + harfL2);
    vertices2[SQUARE_LB] = Vector2(arg_sq2.center.x - harfL2.x, arg_sq2.center.y + harfL2.y);
    // 頂点|rotate
    vertices2[SQUARE_LT] = Rotate2d(arg_sq2.center, vertices2[SQUARE_LT], arg_sq2.radian);
    vertices2[SQUARE_RT] = Rotate2d(arg_sq2.center, vertices2[SQUARE_RT], arg_sq2.radian);
    vertices2[SQUARE_RB] = Rotate2d(arg_sq2.center, vertices2[SQUARE_RB], arg_sq2.radian);
    vertices2[SQUARE_LB] = Rotate2d(arg_sq2.center, vertices2[SQUARE_LB], arg_sq2.radian);

    for (const auto& vertex1 : vertices1) {
        Vector2 point2 = ClosestPoint_OBB(arg_sq2, vertex1);
        float dist = (vertex1 - point2).length();
        if (dist < minDist) {
            minDist = dist;
            closestPoint1 = vertex1;
            closestPoint2 = point2;
        }
    }

    for (const auto& vertex2 : vertices2) {
        Vector2 point1 = ClosestPoint_OBB(arg_sq1, vertex2);
        float dist = (vertex2 - point1).length();
        if (dist < minDist) {
            minDist = dist;
            closestPoint1 = point1;
            closestPoint2 = vertex2;
        }
    }

    return { closestPoint1, closestPoint2 };
}

Vector2 CollisionResponse::PushBack_OBB2OBB_SameRot(const Square& arg_sq1, const Square& arg_sq2)
{
    Vector2 result(0.f, 0.f);

    if (std::abs(arg_sq1.radian) != std::abs(arg_sq2.radian)) { return result; }    // 回転角が一致していない

    //-OBB1-----------------------------------------
    float sin1 = std::sin(arg_sq1.radian);
    float cos1 = std::cos(arg_sq2.radian);
    Matrix4 mRotateZ1
    {
        cos1, sin1, 0.0f, 0.0f,
       -sin1, cos1, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    // ワールド行列宣言
    Matrix4 mWorld1{};
    // ワールド行列初期化
    mWorld1.UnitMatrix();

    // 回転行列の合成
    mWorld1 *= mRotateZ1;
    // 移動行列の合成
    mWorld1.m[3][0] = arg_sq1.center.x;
    mWorld1.m[3][1] = arg_sq1.center.y;

    //-OBB2-----------------------------------------
    float sin2 = std::sin(arg_sq2.radian);
    float cos2 = std::cos(arg_sq2.radian);
    Matrix4 mRotateZ2
    {
        cos2, sin2, 0.0f, 0.0f,
       -sin2, cos2, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    // ワールド行列宣言
    Matrix4 mWorld2{};
    // ワールド行列初期化
    mWorld2.UnitMatrix();

    // 回転行列の合成
    mWorld2 *= mRotateZ2;
    // 移動行列の合成
    mWorld2.m[3][0] = arg_sq2.center.x;
    mWorld2.m[3][1] = arg_sq2.center.y;

    //-計算処理----------------------------------------
    // 逆行列の生成
    Matrix4 mWorld1Inv = mWorld1.Inverse();
    mWorld1 *= mWorld1Inv;      // 回転の打ち消し
    mWorld2 *= mWorld1Inv;      // ローカル座標化

    // AABBに変換
    Square aabb1(Vector2{ mWorld1.m[3][0], mWorld1.m[3][1] }, arg_sq1.length);
    Square aabb2(Vector2{ mWorld2.m[3][0], mWorld2.m[3][1] }, arg_sq2.length);

    const Vector2 pushBack_local = CollisionResponse::PushBack_AABB2AABB(aabb1, aabb2);
    const Vector4 pushBack_world = Vector4(pushBack_local.x, pushBack_local.y, 0, 0) * mWorld1;
    result = Vector2(pushBack_world.x, pushBack_world.y);
    return result;
}

Vector2 CollisionResponse::PushBack_OBB2AABB(const Square& arg_sq1, const Square& arg_sq2)
{
    //-OBB------------------------------------------
    const float rad = arg_sq1.radian;
    const float sin = std::sin(rad);
    const float cos = std::cos(rad);
    Matrix4 obb_matRotZ
    {
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    // ワールド行列宣言
    Matrix4 obb_matWorld{};
    // ワールド行列初期化
    obb_matWorld.UnitMatrix();

    // 回転行列の合成
    obb_matWorld *= obb_matRotZ;
    // 移動行列の合成
    obb_matWorld.m[3][0] = arg_sq1.center.x;
    obb_matWorld.m[3][1] = arg_sq1.center.y;

    //-AABB----------------------------------------
    // ワールド行列宣言
    Matrix4 aabb_matWorld{};
    // ワールド行列初期化
    aabb_matWorld.UnitMatrix();

    // 移動行列の合成
    aabb_matWorld.m[3][0] = arg_sq2.center.x;
    aabb_matWorld.m[3][1] = arg_sq2.center.y;

    //-計算処理----------------------------------------
    // 逆行列の生成
    Matrix4 obb_inv = obb_matWorld.Inverse();
    obb_matWorld *= obb_inv;        // 回転の打ち消し
    aabb_matWorld *= obb_inv;       // ローカル座標化

    // AABBに変換
    Square aabb1(Vector2{ obb_matWorld.m[3][0], obb_matWorld.m[3][1] }, arg_sq1.length);
    Square aabb2(Vector2{ aabb_matWorld.m[3][0], aabb_matWorld.m[3][1] }, arg_sq2.length);

    const Vector2 pushBack_local = CollisionResponse::PushBack_AABB2AABB(aabb1, aabb2);
    const Vector4 pushBack_world = Vector4(pushBack_local.x, pushBack_local.y, 0, 0) * obb_matWorld;
    return Vector2(pushBack_world.x, pushBack_world.y);
}

Vector2 CollisionResponse::PushBack_OBB2Circle(const Square& arg_sq, const Circle& arg_c)
{
    Vector2 result(0.f, 0.f);

    //-OBB------------------------------------------
    const float rad = arg_sq.radian;
    const float sin = std::sin(rad);
    const float cos = std::cos(rad);
    Matrix4 obb_matRotZ
    {
         cos,  sin, 0.0f, 0.0f,
        -sin,  cos, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    // ワールド行列宣言
    Matrix4 obb_matWorld{};
    // ワールド行列初期化
    obb_matWorld.UnitMatrix();

    // 回転行列の合成
    obb_matWorld *= obb_matRotZ;
    // 移動行列の合成
    obb_matWorld.m[3][0] = arg_sq.center.x;
    obb_matWorld.m[3][1] = arg_sq.center.y;

    //-Circle---------------------------------------
    // ワールド行列宣言
    Matrix4 circle_matWorld{};
    // ワールド行列初期化
    circle_matWorld.UnitMatrix();

    // 移動行列の合成
    circle_matWorld.m[3][0] = arg_c.center.x;
    circle_matWorld.m[3][1] = arg_c.center.y;

    //-計算処理----------------------------------------
    // 逆行列の生成
    Matrix4 mWorld1Inv = obb_matWorld.Inverse();
    obb_matWorld *= mWorld1Inv;      // 回転の打ち消し
    circle_matWorld *= mWorld1Inv;      // ローカル座標化

    // AABBに変換
    Square aabb(Vector2{ obb_matWorld.m[3][0], obb_matWorld.m[3][1] }, arg_sq.length);
    Circle circle(Vector2{ circle_matWorld.m[3][0], circle_matWorld.m[3][1] }, arg_c.radius);

    const Vector2 pushBack_local = CollisionResponse::PushBack_AABB2Circle(aabb, circle);
    const Vector4 pushBack_world = Vector4(pushBack_local.x, pushBack_local.y, 0, 0) * obb_matWorld;
    result = Vector2(pushBack_world.x, pushBack_world.y);
    return result;
}

Vector2 CollisionResponse::PushBack_AABB2AABB(const Square& arg_sq1, const Square& arg_sq2)
{
    // ※どちらもAABBとあるように回転していないことを前提としている。
    // ※どちらかが回転している場合はまた別途で考えること。
    //-AABB1----------------------------------------------
    const Vector2 center1 = arg_sq1.center;
    const Vector2 harfL1 = arg_sq1.length / 2;
    const Vector2 min1 = center1 - harfL1;
    const Vector2 max1 = center1 + harfL1;

    //-AABB2----------------------------------------------
    const Vector2 center2 = arg_sq2.center;
    const Vector2 harfL2 = arg_sq2.length / 2;
    const Vector2 min2 = center2 - harfL2;
    const Vector2 max2 = center2 + harfL2;

    // X軸/Y軸においてAABB同士の辺座標を比較
    float overlapX1 = max1.x - min2.x;  // AABB1.右辺とAABB2.左辺
    float overlapX2 = max2.x - min1.x;  // AABB2.右辺とAABB1.左辺
    float overlapY1 = max1.y - min2.y;  // AABB1.下辺とAABB2.上辺
    float overlapY2 = max2.y - min1.y;  // AABB2.下辺とAABB1.上辺

    // 押し戻しする距離
    Vector2 pushBack{};
    // 重複している範囲が小さい軸は X軸/Y軸 どちらか
    if ((std::min)(overlapX1, overlapX2) < (std::min)(overlapY1, overlapY2))
    {
        // X軸:　侵入方向は 右/左 どちらか
        overlapX1 < overlapX2 ?
            pushBack.x = -overlapX1 :
            pushBack.x = overlapX2;
    }
    else
    {
        // Y軸:　侵入方向は 下/上 どちらか
        overlapY1 < overlapY2 ?
            pushBack.y = -overlapY1 :
            pushBack.y = overlapY2;
    }

    return pushBack;
}

Vector2 CollisionResponse::PushBack_AABB2Circle(const Square& arg_sq, const Circle& arg_c)
{
    //-AABB-------------------
    Vector2 harf = arg_sq.length / 2;
    Vector2 sq_min{}, sq_max{};
    sq_min = arg_sq.center - harf;
    sq_max = arg_sq.center + harf;

    // AABB側の最近接点
    Vector2 closestPoint{};
    closestPoint.x = Clamp(arg_c.center.x, sq_min.x, sq_max.x);
    closestPoint.y = Clamp(arg_c.center.y, sq_min.y, sq_max.y);

    // 最近接点=>円の中心までベクトル
    Vector2 diff = arg_c.center - closestPoint;
    // 最近接点=>円の中心までの距離
    float dist = diff.length();

    // 押し戻し距離
    Vector2 pushBack{};
    // 最近接点=>円の中心方向に、めり込んでいる分だけ押し戻す
    pushBack = diff.normalize() * (arg_c.radius - dist);

    return pushBack;
}
