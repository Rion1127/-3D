#pragma once
#include "ICollider.h"
#include "Collision.h"

struct SphereCollider final : public ICollider
{
public:
    //>> コンストラクタ
    SphereCollider(void) = default;
    ~SphereCollider(void) override = default;

    void Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr) override;
    // void Update(void) override {}; アップデートいるか？これ

    //>> 変数
    Sphere sphere_;
};