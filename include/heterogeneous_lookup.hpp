#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {};

struct TransparentStringEqual {};

struct TransparentStringHash {};

struct CompareByAuthor {
    using is_transparent = void; // Разрешает гетерогенный поиск

    bool operator()(const Book& l, const Book& r) const {
        return l.title < r.title;
    }
};

}  // namespace bookdb
