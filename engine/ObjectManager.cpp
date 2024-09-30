#include "ObjectManager.h"
#include "HitStop.h"

void ObjectManager::Initialize(ColliderManager* arg_colliderManagerPtr, Vector2* arg_playerPosPtr)
{
    playerPositionPtr_ = arg_playerPosPtr;

    colliderManagerPtr_ = arg_colliderManagerPtr;
}

void ObjectManager::Update(void)
{
    if (HitStop::GetInstance()->GetIsHitStopping())return;
    // 削除されたオブジェクトたちの更新処理
    Update_ResetQueue();

    for (auto it = objects_.begin(); it != objects_.end();)
    {
        // 更新処理
        (*it)->Update();

        // 死亡判定
        if ((*it)->Get_IsDead() == false) { it++; continue; }

        //// 死亡オブジェクト種類
        //if ((*it)->Get_ObjectType() == OBJECT_BULLET)
        //{
        //    // リソースの解放
        //    (*it).reset();
        //    it = objects_.erase(it);
        //}
        //else
        //{
        //    // 当たり判定を無効とする
        //    auto begin = (*it)->Get_IColliderPtrs()->begin();
        //    auto end = (*it)->Get_IColliderPtrs()->end();
        //    for (auto collider = begin; collider != end; collider++) { (*collider)->Set_IsActive(false); }

        //    // resetキューに移動
        //    objects_resetWait_.push_back(std::move((*it)));
        //    it = objects_.erase(it);
        //}
    }
    
}

void ObjectManager::Draw(void)
{
    for (const auto& o : objects_) { o->Draw(); }
}

void ObjectManager::Update_ResetQueue(void)
{
    for (auto it = objects_resetWait_.begin(); it != objects_resetWait_.end();)
    {
        ////##BOX
        //if ((*it)->Get_ObjectType() == OBJECT_BOX)
        //{
        //    auto box = static_cast<IBox*>((*it).get());
        //    auto dist = (box->GetPos() - *playerPositionPtr_).length();
        //    // 画面外を囲むmin ~ maxのエリアに侵入したら
        //    if (dist > kDist_regeneration_min_ && dist < kDist_regeneration_max_)
        //    {
        //        // 箱を再生成
        //        operator_box_.Add(box->GetBoxType(), box->GetInitPos(), box->GetInitSize(), colliderManagerPtr_);
        //        // 追加したのでこっちのキューからも解放
        //        (*it).reset();
        //        // itを配列から削除
        //        it = objects_resetWait_.erase(it);
        //        // 次のインスタンスへ
        //        continue;
        //    }
        //}

        ////##ENEMY
        //if ((*it)->Get_ObjectType() == OBJECT_ENEMY)
        //{
        //    auto enemy = static_cast<IEnemy*>((*it).get());
        //    auto type = enemy->GetEnemyType();
        //    if (type != EnemyType::ENEMY_BOSS_TWOHAND)
        //    {
        //        auto dist = (enemy->GetInitPos() - *playerPositionPtr_).length();
        //        // 画面外を囲むmin ~ maxのエリアに侵入したら
        //        if (dist > kDist_regeneration_min_ && dist < kDist_regeneration_max_)
        //        {
        //            EnemyInfo_t info;
        //            info.pos = enemy->GetInitPos();
        //            info.size = enemy->GetInitSize();
        //            info.colMPtr = colliderManagerPtr_;
        //            info.dir = enemy->GetEnemyDir();
        //            info.isCanSmash = enemy->GetIsCanSmash();
        //            info.isMove = enemy->GetIsMove();

        //            if (type == ENEMY_CANNON)
        //            {
        //                auto cannon = static_cast<EnemyCannon*>(enemy);
        //                info.bulletSize = cannon->GetBulletSize();
        //                info.bulletRot = cannon->GetBulletRotate();
        //                info.bulletSpeed = cannon->GetBulletVel();
        //                info.bulletInterval = cannon->GetRapidWaitCount();
        //                info.bulletsOperatorPtr = &operator_bullet_;
        //            }
        //            else if (type == ENEMY_SPRING)
        //            {
        //                auto spring = static_cast<EnemySpring*>(enemy);
        //                info.boundPower = spring->GetSpringVel();
        //            }
        //            else if (type == ENEMY_ROLLSQUARE)
        //            {
        //                // こっちでもboundPower取得する必要性あるかも？
        //            }

        //            // 敵を再生成
        //            operator_enemy_.Add(enemy->GetEnemyType(), info);
        //            // 追加したのでこっちのキューからも解放
        //            (*it).reset();
        //            // itを配列から削除
        //            it = objects_resetWait_.erase(it);
        //            // 次のインスタンスへ
        //            continue;
        //        }
        //    }
        //}

        // イテレータを1つ進める
        it++;
    }
}
