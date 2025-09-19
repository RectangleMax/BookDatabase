#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
    namespace Filters {
        // Фильтр по году между min и max
        auto YearBetween(int min, int max) {
            return [min, max](const Book& book) {
                return book.year >= min && book.year <= max;
            };
        }

        // Фильтр по рейтингу выше заданного
        auto RatingAbove(double min_rating) {
            return [min_rating](const Book& book) {
                return book.rating > min_rating;
            };
        }

        // Фильтр по жанру
        auto GenreIs(const std::string& genre) {
            return [genre](const Book& book) {
                return book.genre == GenreFromString(genre);
            };
        }
    }

    namespace Combinators {
        template <typename... Predicates>
        auto all_of(Predicates... preds) {
            return [=](const Book& book) {
                return (preds(book) && ...);
            };
        }

        template <typename... Predicates>
        auto any_of(Predicates... preds) {
            return [=](const Book& book) {
                return (preds(book) || ...);
            };
        }
    }

    
    template<BookIterator Iterator, typename Sentinel, BookPredicate Predicate>
    requires BookSentinel<Sentinel, Iterator>
    std::vector<std::reference_wrapper<const Book>> filterBooks(
        Iterator begin, Sentinel end, Predicate pred) {
        
        std::vector<std::reference_wrapper<const Book>> result;
        
        std::for_each(begin, end, [&](const Book& book) {
            if (pred(book)) {
                result.emplace_back(book);
            }
        });
        
        return result;
    }

}  // namespace bookdb