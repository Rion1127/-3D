#include "ColliderTag.h"

bool ColliderTag::Tag_Search(const std::string& arg_tagName)
{
    // tags_内を全検索
    for (const auto& tag : tags_)
    {
        // みつけた
        if (tag == arg_tagName) { return true; }
    }

    // みつからなかった
    return false;
}
