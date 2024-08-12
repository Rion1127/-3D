#pragma once
#include <functional>
#include "ColliderTag.h"
#include "GameStatus.h"
#include <any>
#include <unordered_map>

class ColliderManager;

class ICollider
{
public:
    //>> 定義
    friend ColliderManager;

    //>> コンストラクタ
    ICollider(void) = default;
    virtual ~ICollider(void) = default;

    //>> 関数
    // 名前の設定/コールバック関数の登録/コライダーマネージャーのptr取得
    virtual void Initialize(const std::string& arg_name, const std::function<void(void)>& arg_callback, ColliderManager* arg_colMPtr);
    virtual void Finalize(void);

    // 自分が衝突したか
    bool IsTrigger_Col(void) { return is_col_ && !is_colPre_; } // 今 == true && 前 == false
    bool IsDetect_Col(void) { return is_col_; }                 // 今 == true
    bool IsRelease_Col(void) { return !is_col_ && is_colPre_; } // 今 == false && 前 == true

    // Id(size_t)と衝突したか
    bool IsTrigger_Id(size_t arg_id) { return IsExist_Cur(arg_id) && !IsExist_Pre(arg_id); } // 今 == true && 前 == false
    bool IsDetect_Id(size_t arg_id) { return IsExist_Cur(arg_id); }                          // 今 == true
    bool IsRelease_Id(size_t arg_id) { return !IsExist_Cur(arg_id) && IsExist_Pre(arg_id); } // 今 == false && 前 == true

    // "Name"と衝突したか
    bool IsTrigger_Name(const std::string& arg_name) { return IsExist_Cur(arg_name) && !IsExist_Pre(arg_name); } // 今 == true && 前 == false
    bool IsDetect_Name(const std::string& arg_name) { return IsExist_Cur(arg_name); }                            // 今 == true
    bool IsRelease_Name(const std::string& arg_name) { return !IsExist_Cur(arg_name) && IsExist_Pre(arg_name); } // 今 == false && 前 == true

    // std::vectorやstd::listといった拡張配列のみ指定可能
    template <template <class, class> class T>
    T<ICollider*, std::allocator<ICollider*>> Extract_Colliders(const std::string& arg_name);
    // 接触した他コライダーを単体で取得。複数ある場合、先頭に検知したものを返す。
    ICollider* Extract_Collider(const std::string& arg_name);

    // ユーザー用|お気に入りコライダー登録関数
    void Bookmark_Add(ICollider* const arg_collierPtr) { bookmarks_.push_back(arg_collierPtr); }
    // ユーザー用|お気に入りコライダー抹消関数
    void Bookmark_Remove(ICollider* const arg_collierPtr) { bookmarks_.remove(arg_collierPtr); }
    // ユーザー用|お気に入りコライダー取得関数
    ICollider* Bookmark_GetCollider(const std::string& arg_name);

    // ユーザー用|伝えたい情報登録関数
    void Data_Add(const std::string& arg_key, const std::any& arg_any) { datas_.emplace(arg_key, arg_any); }
    // ユーザー用|伝えたい情報抹消関数
    void Data_Remove(const std::string& arg_key) { datas_.erase(arg_key); }
    // ユーザー用|伝えたい情報全削除関数
    void Data_Clear(void) { datas_.clear(); }
    // ユーザー用|伝えたい情報取得関数
    template<class T>
    T Data_Get(const std::string& arg_key);
protected:
    virtual void Update(void);

private:
    // リスト内に一致するものがあるか検索を行う
    bool SearchColliders(size_t arg_id, std::list<ICollider*> arg_list);                                 // id
    bool SearchColliders(const std::string& arg_name, std::list<ICollider*> arg_list);                   // 名前

    // コライダーリスト内に一致するものがあるか検索を行う
    bool IsExist_Cur(size_t arg_id) { return SearchColliders(arg_id, colliders_); }                             // 現F id検索
    bool IsExist_Cur(const std::string& arg_name) { return SearchColliders(arg_name, colliders_); }             // 現F 名前検索
    bool IsExist_Pre(size_t arg_id) { return SearchColliders(arg_id, collidersPre_); }                          // 前F id検索
    bool IsExist_Pre(const std::string& arg_name) { return SearchColliders(arg_name, collidersPre_); }          // 前F 名前検索

    void Record_Collider(ICollider* const arg_colPtr) { colliders_.push_back(arg_colPtr); }

    void Execute_UpdateColFlags(void);
    void Execute_Callback(void);

    //>> 変数
    // 重複回避用
    size_t id_{};
    // 識別名
    std::string name_;
    // 有効フラグ
    bool is_active_{};

    // 接触検知
    bool is_col_{};
    bool is_colPre_{};
    // 接触コライダーs
    std::list<ICollider*> colliders_;
    std::list<ICollider*> collidersPre_;
    // コールバック関数
    std::function<void(void)> callback_;
    // お気に入り登録コライダー
    std::list<ICollider*> bookmarks_;
    // タグ
    ColliderTag context_;
    // 伝えたい情報
    std::unordered_map<std::string, std::any> datas_;

protected:
    ColliderManager* colMPtr_{};
    Shape shape_{};

public:
    //>> getter
    bool Get_IsActive(void) { return is_active_; }
    Shape Get_Shape(void) { return shape_; }
    const std::list<ICollider*>& Get_Colliders(void) { return colliders_; }
    const std::string& Get_Name(void) { return name_; }
    // タグ変数にアクセスするため参照渡し
    ColliderTag& Get_Context(void) { return context_; }

    //>> setter
    void Set_IsCol(bool arg_isCol) { is_col_ = arg_isCol; }
    void Set_IsActive(bool arg_isActive) { is_active_ = arg_isActive; }

};

template <template <class, class> class T>
inline T<ICollider*, std::allocator<ICollider*>> ICollider::Extract_Colliders(const std::string& arg_name)
{
    // テンプレートコンテナの生成
    T<ICollider*, std::allocator<ICollider*>> container{};

    // collidersを全検索
    for (const auto& collider : colliders_)
    {
        // 指定された名前と一致していない場合、次のインスタンスを検索
        if (collider->name_ != arg_name) { continue; }
        // 指定された名前のコライダーをコンテナに積む
        container.push_back(collider);
    }

    // 積み終わったコンテナを返す。
    return container;
}

template<class T>
inline T ICollider::Data_Get(const std::string& arg_key)
{
    // 下記両方とも起こりうる。
    // ここで例外スローが発生した場合、keyの値を間違えている可能性がある。
    // ここで例外スローが発生した場合、Data_Get関数を実行しているコライダーが間違えている可能性がある。
    auto& a = datas_[arg_key];
    // ここで例外スローが発生した場合、渡す型を間違えている可能性がある。
    return std::any_cast<T>(a);
}
