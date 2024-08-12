#pragma once
#include <list>
#include <memory>

#include "IObject.h"
#include "ObjectFactory.h"
#include "BoxesOperator.h"
#include "FloorsOperator.h"
#include "GatesOperator.h"
#include "EnemiesOperator.h"
#include "BulletsOperator.h"
#include "ItemsOperator.h"
#include "GimmicksOperator.h"
#include "EventAreasOperator.h"

class StageEditor2;

class ObjectManager
{
public:
    //>> 定義
    friend StageEditor2;
    const float kDist_regeneration_min_ = 850.f;
    const float kDist_regeneration_max_ = 900.f;

    //>> コンストラクタ
    ObjectManager(void) = default;
    ~ObjectManager(void) = default;

    //>> 関数
    void Initialize(ColliderManager* arg_colliderManagerPtr, Vector2* arg_playerPosPtr);
    void Update(void);
    void Draw(void);

    void Register(std::unique_ptr<IObject> arg_obj) { objects_.emplace_back(std::move(arg_obj)); }
    void UnRegister(std::unique_ptr<IObject> arg_obj) { objects_.remove(arg_obj); }

    // 2つの配列を全消去
    void Clear(void) { objects_.clear(); objects_resetWait_.clear(); }

private:
    void Update_ResetQueue(void);

    //>> 変数
    // コライダーマネージャー
    ColliderManager* colliderManagerPtr_;
    // 全てのオブジェクト
    std::list<std::unique_ptr<IObject>> objects_;

    // リセット待機中
    std::list<std::unique_ptr<IObject>> objects_resetWait_;
    Vector2* playerPositionPtr_;

    // オペレーター
    BoxesOperator operator_box_;
    FloorsOperator operator_floor_;
    GatesOperator operator_gate_;
    EnemiesOperator operator_enemy_;
    BulletsOperator operator_bullet_;
    ItemsOperator operator_item_;
    GimmicksOperator operator_gimmick_;
    EventAreasOperator operator_event_;

    //test
    std::unique_ptr<EnemyBossTwoHand> testBoss;

public:
    //>>getter
    BoxesOperator* Get_Operator_Box(void) { return &operator_box_; }
    FloorsOperator* Get_Operator_Floor(void) { return &operator_floor_; }
    GatesOperator* Get_Operator_Gate(void) { return &operator_gate_; }
    EnemiesOperator* Get_Operator_Enemy(void) { return &operator_enemy_; }
    BulletsOperator* Get_Operator_Bullet(void) { return &operator_bullet_; }
    ItemsOperator* Get_Operator_Item(void) { return &operator_item_; }
    GimmicksOperator* Get_Operator_Gimmick(void) { return &operator_gimmick_; }
    EventAreasOperator* Get_Operator_Event(void) { return &operator_event_; }
};

