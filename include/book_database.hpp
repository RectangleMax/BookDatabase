#pragma once

#include <print>
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <boost/container/flat_set.hpp>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"


namespace bookdb {

// template <BookContainerLike BookContainer = std::vector<Book>> // 
template <BookContainerLike BookContainer = boost::container::flat_multiset<Book, CompareByAuthor>>
class BookDatabase {
public:
    // Type aliases

    // Ваш код здесь

    using AuthorContainer = std::set<std::string>; // BookContainer /* Ваш код здесь */;

    BookDatabase() = default;

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods

    // Ваш код здесь

    template<typename... Types>
    void EmplaceBack(Types... args) {
        // books_.emplace_back(args...);
        // books_.insert({args...});
        Book book{args...};
        books_.insert(book);
    }

    typename BookContainer::iterator begin() { return books_.begin(); }
    typename BookContainer::iterator end()   { return books_.end();   }

private:
    BookContainer books_;
    AuthorContainer authors_;
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        /*
        Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        (size/begin/...)

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        */
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
