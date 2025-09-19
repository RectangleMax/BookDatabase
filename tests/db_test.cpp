#include <gtest/gtest.h>

#include "../include/book_database.hpp"
#include "../include/comparators.hpp"
#include "../include/filters.hpp"
#include "../include/statsistics.hpp"

using namespace bookdb;


BookDatabase<> get_base_for_tests() {
    BookDatabase base_1 { {"1984", "George Orwell", 1949, Genre::SciFi, 4., 190}, 
                          {"Animal Farm", "George Orwell", 1945, "Fiction", 4.4, 143} };
    base_1.PushBack({"The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120});
    base_1.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    return base_1;
}

auto base = get_base_for_tests();

TEST(TestComponentName, CheckingAuthorHistogram) {  
    auto how_many_books_by = buildAuthorHistogramFlat(base);
    EXPECT_EQ(how_many_books_by["George Orwell"], 2);
    EXPECT_EQ(how_many_books_by["Leo Tolstoy"], 0);
}

TEST(TestComponentName, CheckingGenreRating) {  
    auto genreRating = calculateGenreRatings(base);
    EXPECT_DOUBLE_EQ(genreRating[Genre::SciFi], 4.25);
    EXPECT_EQ(genreRating.size(), 2);

    EXPECT_DOUBLE_EQ(calculateAverageRating(base), 4.35);
    
    BookDatabase empty_base;
    EXPECT_TRUE(std::isnan(calculateAverageRating(empty_base)));
}

TEST(TestComponentName, CheckRandomSamples) {
    std::size_t sample_size = 2;  
    auto randomSample = sampleRandomBooks(base, sample_size);
    auto copy_base = base;
    std::for_each(randomSample.begin(), randomSample.end(), 
        [&copy_base](const auto& book_ref) {
            auto it = std::find(copy_base.begin(), copy_base.end(), book_ref.get());           
            if (it == copy_base.end())
                EXPECT_TRUE(false);
            copy_base.erase(it);
        }
    );
    EXPECT_EQ(sample_size, randomSample.size());
}

TEST(TestComponentName, CheckTopRating) {
    auto topBooksRef = getTopNBy(base, 2);
    BookDatabase topBase;
    std::copy(topBooksRef.begin(), topBooksRef.end(), std::back_inserter(topBase));
    EXPECT_DOUBLE_EQ(calculateAverageRating(topBase), 4.5);
}

TEST(TestComponentName, CheckFilters) {
    auto filter_one = Combinators::any_of(
        Filters::YearBetween(1947, 2020),
        Filters::RatingAbove(4.45)
    );
    
    auto filter_two = Combinators::all_of(
        Filters::YearBetween(-5, 1930),
        Filters::GenreIs("Fiction")
    );

    EXPECT_EQ(filterBooks(base.begin(), base.end(), filter_one).size(), 3);
    EXPECT_EQ(filterBooks(base.begin(), base.end(), filter_two).size(), 1);
}
