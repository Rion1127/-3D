#include "ICollider.h"
#include "ColliderManager.h"
#include <assert.h>

void ICollider::Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr)
{
    // メンバへの代入
    name_ = arg_name;
    callback_ = arg_callback;
    colMPtr_ = arg_colMPtr;
    assert(colMPtr_);
    colMPtr_->Register(this);

    // コライダーを有効にする
    is_active_ = true;
}

ICollider* ICollider::Extract_Collider(const std::string& arg_name)
{
    // colliders_を全検索
    for (const auto& collider : colliders_)
    {
        // 指定された名前と一致ししだい返却する ※複数ある場合、1つ見つかり次第返却して関数終了
        if (collider->name_ == arg_name) { return collider; }
    }
    return nullptr;
}

ICollider* ICollider::Bookmark_GetCollider(const std::string& arg_name)
{
    // bookmarksを全検索
    for (const auto& collider : bookmarks_)
    {
        // 指定された名前と一致ししだい返却する ※複数ある場合、1つ見つかり次第返却して関数終了
        if (collider->name_ == arg_name) { return collider; }
    }
    return nullptr;
}

void ICollider::Update(void)
{
   /*==想定している処理順==（全てColliderManagerでの実行を行う）
    *
    *　・Execute_UpdateColFlags()の実行                  （衝突フラグやコライダーptrの保存と、それらの初期化）
    *  ↓
    *　・衝突検知関数の実行（ColliderManager側の関数）
    *  ↓
    *　・if(衝突検知）=> Set_IsCol()/Record_Col等の実行    （衝突フラグを立てる/衝突したコライダーptrの記録)
    *  ↓
    *　・Execute_Callback()の実行                        （衝突時に実行する処理）
    * 
    */
}

void ICollider::Finalize(void)
{
    // デフォルトコンストラクタ生成時の例外スロー回避用
    if (!colMPtr_) { return; }

    colMPtr_->UnRegister(this);
}

void ICollider::Execute_UpdateColFlags(void)
{
    // 1フレーム前の結果を保存
    collidersPre_ = colliders_; // 配列: コライダーptrのlist
    is_colPre_ = is_col_;       // フラグ: 衝突判定のbool

    // 現在フレームの結果用に初期化
    is_col_ = false;
    colliders_.clear();
}

void ICollider::Execute_Callback(void)
{
    // nullptrなら実行しない
    if (!callback_) { return; }

    // 実行
    callback_();
}

bool ICollider::SearchColliders(size_t arg_id, std::list<ICollider*> arg_list)
{
    // IDで全検索
    for (const ICollider* const col : arg_list)
    {
        // 名前が一致していればtrue
        if (col->id_ == arg_id) { return true; }
    }

    return false;
}

bool ICollider::SearchColliders(const std::string& arg_name, std::list<ICollider*> arg_list)
{
    // 名前で全検索
    for (const ICollider* const col : arg_list)
    {
        // 名前が一致していればtrue
        if (col->name_ == arg_name) { return true; }
    }

    return false;
}
