#pragma once
#include "ICollider.h"
#include "Collision.h"

class CircleCollider final : public ICollider
{
public:
    //>> コンストラクタ
    CircleCollider(void) = default;
    ~CircleCollider(void) override = default;

    void Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr) override;

    //>> 変数
    Circle circle_;
};

