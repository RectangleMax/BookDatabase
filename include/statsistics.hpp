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

#include <format>
#include <sstream>
#include <iomanip>

#include "concepts.hpp"

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &library, Comparator comp = {}) {
    boost::container::flat_map< std::string, std::size_t, Comparator > num_books_by_author;
    std::for_each(library.cbegin(), library.cend(), [&num_books_by_author] (const Book& book) { ++num_books_by_author[std::string(book.author)]; });
    return num_books_by_author;
}


template<BookIterator Iterator>
auto calculateGenreRatings(Iterator begin_, Iterator end_) {
   
    boost::container::flat_map<Genre, std::pair<double, std::size_t>> genreStats;
    std::for_each(begin_, end_, [&genreStats] (const Book& book) { 
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
auto calculateGenreRatings(const BookDatabase<T> &library) {
    return calculateGenreRatings(library.cbegin(), library.cend());
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
    std::sample(library.cbegin(), library.cend(), std::back_inserter(result),
               sample_size, std::mt19937{std::random_device{}()});
    
    return result;
}

template <BookContainerLike T, typename Comparator = comp::LessByPopularity>
auto getTopNBy(BookDatabase<T>& library, std::size_t sample_size, Comparator comparator = comp::LessByPopularity {}) 
    -> std::vector<std::reference_wrapper<const typename BookDatabase<T>::value_type>> 
{
    using BookRef = std::reference_wrapper<const typename BookDatabase<T>::value_type>;
    std::vector<BookRef> result;
    
    if (library.empty() || sample_size == 0) {
        return result;
    }
    
    // Создаем вектор ссылок на все книги
    std::vector<BookRef> all_books;
    all_books.reserve(library.size());
    for (const auto& book : library) {
        all_books.emplace_back(book);
    }
    
    // Используем частичную сортировку для выбора топ-N элементов
    if (sample_size >= library.size()) {
        // Если нужно больше элементов, чем есть, возвращаем все отсортированные
        std::sort(all_books.begin(), all_books.end(), comparator); //
        return all_books;
    }
    
    // Используем std::partial_sort для эффективного выбора топ-N
    std::partial_sort(all_books.begin(), all_books.begin() + sample_size, 
                all_books.end(), comparator);
    
    // Копируем топ-N элементов в результат
    result.assign(all_books.begin(), all_books.begin() + sample_size);
    return result;
}

}  // namespace bookdb

namespace std {

template <>
struct formatter<boost::container::flat_map< std::string, std::size_t, bookdb::TransparentStringLess >> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const boost::container::flat_map< std::string, std::size_t, bookdb::TransparentStringLess >& num_books_by_author, format_context& ctx) const {
        std::stringstream ss;
        const int left_shift = 25;
        const int right_shift = 7;
        ss << "\n";
        ss << std::setw(left_shift) << "Author" << " / number of books\n";
        ss << "----------------------------------------------\n";
        std::for_each(num_books_by_author.begin(), num_books_by_author.end(), 
            [&ss] (const auto& pair) {ss << std::setw(left_shift) << pair.first << " / " << std::setw(right_shift) << pair.second << "\n"; });
        return format_to(ctx.out(), "{}", ss.str());
    }
};

template <>
struct formatter< boost::container::flat_map< bookdb::Genre, double, std::less<>> > {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const boost::container::flat_map< bookdb::Genre, double, std::less<>>& average_rating_by_genre, format_context& ctx) const {
        std::stringstream ss;
        const int left_shift = 15;
        const int right_shift = 7;
        ss << "\n";
        ss << std::setw(left_shift) << "Genre" << " / Average rating\n";
        ss << "----------------------------------------------\n";
        std::for_each(average_rating_by_genre.begin(), average_rating_by_genre.end(), 
            [&ss] (const auto& pair) {ss << std::setw(left_shift) << std::format("{}", pair.first) << " / " << std::setw(right_shift) << pair.second << "\n"; });
        return format_to(ctx.out(), "{}", ss.str());
    }
};

}