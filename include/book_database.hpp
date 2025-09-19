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

template <BookContainerLike BookContainer = std::vector<Book>> // 
class BookDatabase {
public:
    // Type aliases

    // Ваш код здесь

    using AuthorContainer = std::set<std::string, std::less<>>; // BookContainer /* Ваш код здесь */;

    BookDatabase() = default;

    BookDatabase(std::initializer_list<Book> init_list) : 
                    books_(init_list.begin(), init_list.end())  {
        std::for_each(books_.begin(), books_.end(), [this] (const auto& the_book) {
            auto [it, is_emplaced] = authors_.emplace(the_book.author);
            the_book.author = *it;
        });
    }

    
    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods
    using value_type = typename BookContainer::value_type;
    using iterator =  typename BookContainer::iterator;
    using const_iterator =  typename BookContainer::const_iterator;
    iterator begin()  { return books_.begin();  }
    iterator end()    { return books_.end();    }
    const_iterator cbegin() const { return books_.cbegin(); }
    const_iterator cend()   const { return books_.cend();   }

    

    bool empty() const { return books_.empty(); }
    void erase(BookContainer::iterator& it) { books_.erase(it); }
    std::size_t size() const { return books_.size(); }

    template<typename... OtherArgs>
    void EmplaceBack(const std::string& t, std::string_view a, OtherArgs&&... args) {
        auto [it, is_emplaced] = authors_.emplace(a);
        books_.emplace_back(t, *it, std::forward<OtherArgs>(args)...);
    }

    void PushBack(const Book& book) {
        auto [it, is_emplaced] = authors_.emplace(book.author);
        book.author = *it;
        books_.push_back(book);
    }

    void push_back(const Book& book) {
        PushBack(book);
    }

    const BookContainer& GetBooks() const {
        return books_;
    }

    const AuthorContainer& GetAuthors() const {
        return authors_;
    }

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
        */ 
        

        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }
        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
