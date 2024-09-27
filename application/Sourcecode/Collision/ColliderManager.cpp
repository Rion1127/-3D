#include "ColliderManager.h"
#include "CollisionChecker.h"

#include "RectCollider.h"
#include "CircleCollider.h"
#include "SphereCollider.h"
#include "HitStop.h"

void ColliderManager::Update(void)
{
    if (HitStop::GetInstance()->GetIsHitStopping())return;
    // フラグやptrの更新処理なので、衝突検知と分けて行う。
    for (const auto& collider : colliders_)
    {
        // IColliderのUpdate()内に記述されている、処理プロセスを番号形式でこちらにも記載
        // 1.衝突フラグやコライダーptrの保存と、それらの初期化
        collider->Execute_UpdateColFlags();
    }

    // 衝突検知
    for (auto it1 = colliders_.begin(); it1 != colliders_.end(); ++it1)
    {
        // コライダーが有効になっているか
        if ((*it1)->Get_IsActive() == false) { continue; }


        // it1の1つ先のイテレータ
        auto it2 = it1;
        it2++;
        // ++it1 ~ colliders_.end()までを全検索
        for (; it2 != colliders_.end(); ++it2)
        {
            // コライダーが有効になっているか
            if ((*it2)->Get_IsActive() == false) { continue; }

            // 2.it1とit2の衝突判定を確認
            if ((*it1)->shape_ < SHAPE_3D || (*it2)->shape_ < SHAPE_3D) {
                Collision2d(*it1, *it2); // <-No.3は上記のCollision()内にて行われている。
            }
            else {
                Collision3d(*it1, *it2); // <-No.3は上記のCollision()内にて行われている。
            }
        }

        // 4.callbackの実行
        (*it1)->Execute_Callback();
    }
}

void ColliderManager::Register(ICollider* arg_collider)
{
    // Idの割り当て
    arg_collider->id_ = countId_;
    // 次用のIdをずらす
    countId_++;

    // リストに登録
    colliders_.push_back(arg_collider);
}

void ColliderManager::UnRegister(ICollider* arg_collider)
{
    // リストから抹消
    colliders_.remove(arg_collider);
}
///////////////////////
//2Dの当たり判定
void ColliderManager::Collision2d(ICollider* arg_col1, ICollider* arg_col2)
{
    bool isCol = false;
    bool temp = false;

    // 矩形と矩形
    temp = Rect2Rect(arg_col1, arg_col2);
    isCol = (std::max)(isCol, temp);        // trueがfalseに書き換わらないように
    // 矩形と円
    temp = Rect2Circle(arg_col1, arg_col2);
    isCol = (std::max)(isCol, temp);        // trueがfalseに書き換わらないように

    if (!isCol) { return; }

    // 3.衝突判定フラグや接触相手のptrを取得
    arg_col1->Set_IsCol(true);
    arg_col1->Record_Collider(arg_col2);

    arg_col2->Set_IsCol(true);
    arg_col2->Record_Collider(arg_col1);
}

bool ColliderManager::Rect2Rect(ICollider* arg_col1, ICollider* arg_col2)
{
    bool isCorrectShape = (arg_col1->Get_Shape() == SHAPE_RECT) && (arg_col2->Get_Shape() == SHAPE_RECT);
    if (!isCorrectShape) { return false; }

    RectCollider* rect1 = static_cast<RectCollider*>(arg_col1);
    RectCollider* rect2 = static_cast<RectCollider*>(arg_col2);

    bool isCol = CollisionChecker::Check_OBB2OBB(rect1->square_, rect2->square_);
    return isCol;
}

bool ColliderManager::Rect2Circle(ICollider* arg_col1, ICollider* arg_col2)
{
    bool isCorrectShape1 = (arg_col1->Get_Shape() == SHAPE_RECT) && (arg_col2->Get_Shape() == SHAPE_CIRCLE);
    bool isCorrectShape2 = (arg_col1->Get_Shape() == SHAPE_CIRCLE) && (arg_col2->Get_Shape() == SHAPE_RECT);
    // どちらもfalseなら、関数スキップ
    if (!isCorrectShape1 && !isCorrectShape2) { return false; }

    if (isCorrectShape1)
    {
        RectCollider* rect = static_cast<RectCollider*>(arg_col1);
        CircleCollider* circle = static_cast<CircleCollider*>(arg_col2);

        return CollisionChecker::Check_OBB2Circle(rect->square_, circle->circle_);
    }
    else
    {
        CircleCollider* circle = static_cast<CircleCollider*>(arg_col1);
        RectCollider* rect = static_cast<RectCollider*>(arg_col2);

        return CollisionChecker::Check_OBB2Circle(rect->square_, circle->circle_);
    }
}
///////////////////////

///////////////////////
//3Dの当たり判定
void ColliderManager::Collision3d(ICollider* arg_col1, ICollider* arg_col2)
{
    bool isCol = false;
    bool temp = false;

    // 矩形と矩形
    temp = Sphere2Sphere(arg_col1, arg_col2);
    isCol = (std::max)(isCol, temp);        // trueがfalseに書き換わらないように

    if (!isCol) { return; }

    // 3.衝突判定フラグや接触相手のptrを取得
    arg_col1->Set_IsCol(true);
    arg_col1->Record_Collider(arg_col2);

    arg_col2->Set_IsCol(true);
    arg_col2->Record_Collider(arg_col1);
}

bool ColliderManager::Sphere2Sphere(ICollider* arg_col1, ICollider* arg_col2)
{
    bool isCorrectShape = (arg_col1->Get_Shape() == SHAPE_SPHERE) && (arg_col2->Get_Shape() == SHAPE_SPHERE);
    if (!isCorrectShape) { return false; }

    SphereCollider* rect1 = static_cast<SphereCollider*>(arg_col1);
    SphereCollider* rect2 = static_cast<SphereCollider*>(arg_col2);
    bool isCol = BallCollision(rect1->sphere_, rect2->sphere_);
    return isCol;
}
///////////////////////

