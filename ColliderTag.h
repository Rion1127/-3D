#pragma once
#include <list>
#include <string>

class ColliderTag
{
public:
    //>> コンストラクタ
    ColliderTag(void) = default;
    ~ColliderTag(void) = default;

    //>> 関数
    // タグ|追加
    void Tag_Add(const std::string& arg_tagName) { tags_.push_back(arg_tagName); }
    // タグ|指定削除
    void Tag_Remove(const std::string& arg_tagName) { tags_.remove(arg_tagName); }
    // タグ|全削除
    void Tag_Clear(void) { tags_.clear(); }
    // タグ|存在検索
    bool Tag_IsExist(const std::string& arg_tagName) { return Tag_Search(arg_tagName); }

private:
    // タグ|存在検索（隠蔽）
    bool Tag_Search(const std::string& arg_tagName);

    //>> 変数
    std::list<std::string> tags_;
};

