#pragma once
#include "ICollider.h"

class ColliderManager
{
public:
    //>> コンストラクタ
    ColliderManager(void) = default;
    ~ColliderManager(void) = default;

    //>> 関数
    void Update(void);
    
    void Register(ICollider* arg_collider);
    void UnRegister(ICollider* arg_collider);

private:
    void Collision(ICollider* arg_col1, ICollider* arg_col2);

    //--Circle_to----------------------------------------
    bool Circle2Circle(ICollider* arg_col1, ICollider* arg_col2);
    bool Circle2Rect(ICollider* arg_col1, ICollider* arg_col2);

    //--Rect_to------------------------------------------
    bool Rect2Rect(ICollider* arg_col1, ICollider* arg_col2);
    bool Rect2Circle(ICollider* arg_col1, ICollider* arg_col2);

    //>> 変数
    std::list<ICollider*> colliders_;
    size_t countId_ = 1; // ※0スタートはありえない
};

