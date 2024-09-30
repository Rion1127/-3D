#pragma once
#include <list>
#include <memory>

#include "IObject.h"

class ObjectManager
{
public:
    //>> 定義
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
public:
    //>>getter
};

