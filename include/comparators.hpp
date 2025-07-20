#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book& l, const Book& r) { return l.author < r.author; }
};

}  // namespace bookdb::comp