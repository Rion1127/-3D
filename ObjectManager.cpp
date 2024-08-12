#include "ObjectManager.h"
#include "DeadEvent.h"
#include "EnemyPopEvent.h"
#include "ArrowSignboard.h"
#include "HitStop.h"
#include "ChangeBGMEvent.h"

void ObjectManager::Initialize(ColliderManager* arg_colliderManagerPtr, Vector2* arg_playerPosPtr)
{
    playerPositionPtr_ = arg_playerPosPtr;

    // オペレーターの初期化
    operator_box_.Initialize(&objects_);
    operator_floor_.Initialize(&objects_);
    operator_gate_.Initialize(&objects_);
    operator_enemy_.Initialize(&objects_, &operator_bullet_);
    operator_bullet_.Initialize(&objects_);
    operator_item_.Initialize(&objects_);
    operator_gimmick_.Initialize(&objects_);
    operator_event_.Initialize(&objects_);

    colliderManagerPtr_ = arg_colliderManagerPtr;
    operator_floor_.Add(FLOOR_PLATFORM, { 200,800 }, { 1000,100 }, 0.f, arg_colliderManagerPtr);
    operator_floor_.Add(FLOOR_MOVE, { 200,700 }, { 100,50 }, 0.f, arg_colliderManagerPtr);
    operator_floor_.Add(FLOOR_NORMAL, { 200,1000 }, { 1000,100 }, 0.f, arg_colliderManagerPtr);
    operator_floor_.Add(FLOOR_NORMAL, { -300,500 }, { 100,1000 }, 0.f, arg_colliderManagerPtr);
    operator_floor_.Add(FLOOR_NORMAL, { 700,500 }, { 100,1000 }, 0.f, arg_colliderManagerPtr);
    EnemyInfo_t enemyInfo;
    enemyInfo.bulletSize = { 64,64 };
    enemyInfo.bulletsOperatorPtr = &operator_bullet_;
    enemyInfo.colMPtr = arg_colliderManagerPtr;
    enemyInfo.dir = DIR_RIGHT;
    enemyInfo.isCanSmash = true;
    enemyInfo.pos = { 200,700 };
    enemyInfo.size = { 64,64 };
    enemyInfo.bulletInterval = 120;
    enemyInfo.isMove = true;
    //operator_enemy_.Add(ENEMY_CANNON, enemyInfo);
    //operator_enemy_.Add(ENEMY_ROLLSQUARE, enemyInfo);
    /*enemyInfo.pos = { 0,700 };
    operator_enemy_.Add(ENEMY_WALK, enemyInfo);
    enemyInfo.pos = { 100,700 };
    operator_enemy_.Add(ENEMY_WALK, enemyInfo);*/
    /*enemyInfo.pos = { 200,700 };
    operator_enemy_.Add(ENEMY_FLY_U, enemyInfo);
    enemyInfo.pos = { 250,650 };
    operator_enemy_.Add(ENEMY_FLY_U, enemyInfo);
    enemyInfo.pos = { 50,750 };
    operator_enemy_.Add(ENEMY_FLY_U, enemyInfo);*/
    /*testBoss = std::make_unique<EnemyBossTwoHand>();
    testBoss->Initialize({ 950,-400 }, { 128,128 }, 0, arg_colliderManagerPtr);
    testBoss->SetPlayerPos(playerPositionPtr_);
    testBoss->SetEnemiesOperator(&operator_enemy_);*/


    /*std::unique_ptr<DeadEvent> event = std::make_unique<DeadEvent>();
    event->Initialize({ 200,800 }, { 1000,100 }, 0, {0,300}, arg_colliderManagerPtr);
    objects_.emplace_back(std::move(event));*/
    /*std::unique_ptr<EnemyPopEvent> event = std::make_unique<EnemyPopEvent>();
    event->Initialize(ENEMY_FLY_I , enemyInfo ,{ 200,800 }, { 1000,100 }, 0, &operator_enemy_, arg_colliderManagerPtr);
    objects_.emplace_back(std::move(event));*/

    //std::unique_ptr<ArrowSignboard> board = std::make_unique<ArrowSignboard>();
    //board->Initialize({ 105,600 }, { 64,64 }, DIR_RIGHT, arg_colliderManagerPtr);
    //objects_.emplace_back(std::move(board));

    std::unique_ptr<ChangeBGMEvent> event = std::make_unique<ChangeBGMEvent>();
    event->Initialize({ 0,800 }, { 200,100 }, 0, "BossBattleBGM", arg_colliderManagerPtr);
    objects_.emplace_back(std::move(event));
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

        // 死亡オブジェクト種類
        if ((*it)->Get_ObjectType() == OBJECT_BULLET)
        {
            // リソースの解放
            (*it).reset();
            it = objects_.erase(it);
        }
        else
        {
            // 当たり判定を無効とする
            auto begin = (*it)->Get_IColliderPtrs()->begin();
            auto end = (*it)->Get_IColliderPtrs()->end();
            for (auto collider = begin; collider != end; collider++) { (*collider)->Set_IsActive(false); }

            // resetキューに移動
            objects_resetWait_.push_back(std::move((*it)));
            it = objects_.erase(it);
        }
    }
    /* if (testBoss != nullptr) {
         testBoss->Update();
         if (testBoss->GetIsDelete()) {
             testBoss.reset();
         }
     }*/
}

void ObjectManager::Draw(void)
{
    for (const auto& o : objects_) { o->Draw(); }
    /*if (testBoss != nullptr) {
        testBoss->Draw();
    }*/
}

void ObjectManager::Update_ResetQueue(void)
{
    for (auto it = objects_resetWait_.begin(); it != objects_resetWait_.end();)
    {
        //##BOX
        if ((*it)->Get_ObjectType() == OBJECT_BOX)
        {
            auto box = static_cast<IBox*>((*it).get());
            auto dist = (box->GetPos() - *playerPositionPtr_).length();
            // 画面外を囲むmin ~ maxのエリアに侵入したら
            if (dist > kDist_regeneration_min_ && dist < kDist_regeneration_max_)
            {
                // 箱を再生成
                operator_box_.Add(box->GetBoxType(), box->GetInitPos(), box->GetInitSize(), colliderManagerPtr_);
                // 追加したのでこっちのキューからも解放
                (*it).reset();
                // itを配列から削除
                it = objects_resetWait_.erase(it);
                // 次のインスタンスへ
                continue;
            }
        }

        //##ENEMY
        if ((*it)->Get_ObjectType() == OBJECT_ENEMY)
        {
            auto enemy = static_cast<IEnemy*>((*it).get());
            auto type = enemy->GetEnemyType();
            if (type != EnemyType::ENEMY_BOSS_TWOHAND)
            {
                auto dist = (enemy->GetInitPos() - *playerPositionPtr_).length();
                // 画面外を囲むmin ~ maxのエリアに侵入したら
                if (dist > kDist_regeneration_min_ && dist < kDist_regeneration_max_)
                {
                    EnemyInfo_t info;
                    info.pos = enemy->GetInitPos();
                    info.size = enemy->GetInitSize();
                    info.colMPtr = colliderManagerPtr_;
                    info.dir = enemy->GetEnemyDir();
                    info.isCanSmash = enemy->GetIsCanSmash();
                    info.isMove = enemy->GetIsMove();

                    if (type == ENEMY_CANNON)
                    {
                        auto cannon = static_cast<EnemyCannon*>(enemy);
                        info.bulletSize = cannon->GetBulletSize();
                        info.bulletRot = cannon->GetBulletRotate();
                        info.bulletSpeed = cannon->GetBulletVel();
                        info.bulletInterval = cannon->GetRapidWaitCount();
                        info.bulletsOperatorPtr = &operator_bullet_;
                    }
                    else if (type == ENEMY_SPRING)
                    {
                        auto spring = static_cast<EnemySpring*>(enemy);
                        info.boundPower = spring->GetSpringVel();
                    }
                    else if (type == ENEMY_ROLLSQUARE)
                    {
                        // こっちでもboundPower取得する必要性あるかも？
                    }

                    // 敵を再生成
                    operator_enemy_.Add(enemy->GetEnemyType(), info);
                    // 追加したのでこっちのキューからも解放
                    (*it).reset();
                    // itを配列から削除
                    it = objects_resetWait_.erase(it);
                    // 次のインスタンスへ
                    continue;
                }
            }
        }

        // イテレータを1つ進める
        it++;
    }
}
