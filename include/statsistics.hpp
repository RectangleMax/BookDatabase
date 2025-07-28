#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <boost/container/flat_map.hpp>
#include <map>

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    boost::container::flat_map< std::string, std::size_t, Comparator > num_books_by_author;
    std::for_each(cont.cbegin(), cont.cend(), [&num_books_by_author] (const Book& book) { ++num_books_by_author[std::string(book.author)]; });
    return num_books_by_author;
}

void displayAuthorHistogram(const boost::container::flat_map< std::string, std::size_t, TransparentStringLess >& num_books_by_author) {
    std::println("Author / Num of books");
    std::for_each(num_books_by_author.cbegin(), num_books_by_author.cend(), 
            [] (const std::pair<std::string, std::size_t>& p) { std::println("{} / {}", p.first, p.second); });
}

template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &cont) {
    std::map<Genre, std::pair<double, std::size_t>> sum_rating_by_genre;
    std::for_each(cont.cbegin(), cont.cend(), [&sum_rating_by_genre] (const Book& book) { 
        auto [it, is_emplaced] = sum_rating_by_genre.try_emplace(book.genre, book.rating, 1);
        if (!is_emplaced) {
            it->second.first += book.rating;
            ++(it->second.second);
        }
    });

    boost::container::flat_map< Genre, double, std::less<> > average_rating_by_genre;
    std::transform(sum_rating_by_genre.cbegin(), sum_rating_by_genre.cend(), 
        std::inserter(average_rating_by_genre, average_rating_by_genre.begin()),
        [](const auto& pair) { std::make_pair(pair.first, pair.second.first / pair.second.second); });
    return average_rating_by_genre;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.size() == 0)
        return 0;
    double sum_ = 0;
    std::for_each(cont.cbegin(), cont.cend(), [&sum_](const Book& book) { sum_ += book.rating; }); 
    return sum_ / cont.size();
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &cont) {
    if (cont.size() == 0)
        return 0;
    double sum_ = 0;
    std::for_each(cont.cbegin(), cont.cend(), [&sum_](const Book& book) { sum_ += book.rating; }); 
    return sum_ / cont.size();
}

}  // namespace bookdb
