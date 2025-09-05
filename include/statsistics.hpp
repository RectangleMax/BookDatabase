#pragma once

#include <algorithm>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>
#include <boost/container/flat_map.hpp>
#include <map>
#include <cmath> // std::nan

#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &library, Comparator comp = {}) {
    boost::container::flat_map< std::string, std::size_t, Comparator > num_books_by_author;
    std::for_each(library.cbegin(), library.cend(), [&num_books_by_author] (const Book& book) { ++num_books_by_author[std::string(book.author)]; });
    return num_books_by_author;
}


template <BookContainerLike T>
auto calculateGenreRatings(const BookDatabase<T> &library) {
    boost::container::flat_map<Genre, std::pair<double, std::size_t>> genreStats;
    std::for_each(library.cbegin(), library.cend(), [&genreStats] (const Book& book) { 
        auto& stats = genreStats[book.genre];
        stats.first += book.rating;
        ++stats.second;
    });

   boost::container::flat_map< Genre, double, std::less<> > average_rating_by_genre;
    std::transform(genreStats.cbegin(), genreStats.cend(), 
        std::inserter(average_rating_by_genre, average_rating_by_genre.begin()),
        [](const auto& pair) { return std::make_pair(pair.first, pair.second.first / pair.second.second); });

    return average_rating_by_genre;
}


template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &library) {
    if (library.size() == 0)
        return std::nan("");
    double sum_ = 0;
    std::for_each(library.cbegin(), library.cend(), [&sum_](const Book& book) { sum_ += book.rating; }); 
    return sum_ / library.size();
}


template <BookContainerLike T>
std::vector<std::reference_wrapper<const Book>> 
sampleRandomBooks(const BookDatabase<T>& library, std::size_t sample_size) {
    std::vector<std::reference_wrapper<const Book>> result;
    
    if (sample_size >= library.size()) { // Возвращаем все книги
        result.reserve(library.size());
        std::transform(library.cbegin(), library.cend(), std::back_inserter(result),
                      [](const Book& book) { return std::cref(book); });
        return result;
    }
    
    result.reserve(sample_size);
    
    // Используем std::sample с std::random_device
    std::random_device rd;
    std::sample(library.cbegin(), library.cend(), std::back_inserter(result),
               sample_size, std::mt19937{rd()});
    
    return result;
}

}  // namespace bookdb
