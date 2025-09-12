#pragma once

#include <concepts>
#include <iterator>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = true;

template <typename T>
concept BookIterator = 
    std::bidirectional_iterator<T> &&
    (std::same_as<std::iter_value_t<T>, Book> ||
     std::same_as<std::iter_reference_t<T>, Book&>);

template <typename S, typename Iterator>
concept BookSentinel = requires(const Iterator& iter, const S& sentinel) {
    requires BookIterator<Iterator>;
    requires std::semiregular<S>;
    { iter == sentinel } -> std::convertible_to<bool>;
    { sentinel == iter } -> std::convertible_to<bool>;
};


template <typename P>
concept BookPredicate = requires(P predicate, const Book& book) {
    { predicate(book) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comp, const Book& book1, const Book& book2, 
                                 const std::string& str, const char* cstr) {
    { comp(book1, book2) } -> std::convertible_to<bool>;
};


}  // namespace bookdb