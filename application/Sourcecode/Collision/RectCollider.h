#pragma once
#include "ICollider.h"
#include "Collision.h"

struct RectCollider final : public ICollider
{
public:
    //>> コンストラクタ
    RectCollider(void) = default;
    ~RectCollider(void) override = default;

    void Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr) override;
    // void Update(void) override {}; アップデートいるか？これ

    //>> 変数
    Square square_;
};

