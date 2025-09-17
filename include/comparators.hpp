#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const Book& l, const Book& r) { return l.author > r.author; }
};

struct LessByPopularity {
    bool operator()(const Book& l, const Book& r) { return l.rating > r.rating; }
};

using LessByRating = LessByPopularity;

}  // namespace bookdb::comp