#include <gtest/gtest.h>
#include "get_base_for_tests.h"

auto base = get_base_for_tests();
BookDatabase empty_base;

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
    EXPECT_TRUE(std::isnan(calculateAverageRating(empty_base)));
}

TEST(TestComponentName, CheckRandomSamples) {
    std::size_t sample_size = 2;  
    auto randomSample = sampleRandomBooks(base, 2);
    // for (const auto& book : randomSample) {
    //     std::cout << book.get().title << " ";
    // }
    // std::cout << std::endl;
    EXPECT_EQ(sample_size, randomSample.size());
}

