#pragma once
#include <cstdint>
#include <vector>
#include "ICollider.h"
#include "GameStatus.h"

//class ObjectManager;

class IObject
{
public:
    //>> 定義
    //friend ObjectManager;

    //>> コンストラクタ
    IObject(void) = default;
    virtual ~IObject(void) = default;

    //>> 関数
    virtual void Update(void) {}
    virtual void Draw(void) {}

private:
    //>> 変数
    uint32_t id_{};
    // 生成するオブジェクトの種類
    ObjectType kind_;
    //
    std::vector<ICollider*> iColliderPtrs_;

protected:
    // 死亡フラグ
    bool isDead_{};

public:
    //>> getter
    ObjectType Get_ObjectType(void) const { return kind_; }
    bool Get_IsDead(void) const { return isDead_; }
    std::vector<ICollider*>* Get_IColliderPtrs(void) { return &iColliderPtrs_; }

protected:
    //>> setter
    // オブジェクトの種類を設定する関数、継承先クラスのコンストラクタ内でのみ使う事。
    void Set_ObjectType(ObjectType arg_objType) { kind_ = arg_objType; }
    void Add_ColliderPtr(ICollider* arg_iColliderPtr) { iColliderPtrs_.push_back(arg_iColliderPtr); }
};
