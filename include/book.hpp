#pragma once

#include <format>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

constexpr Genre GenreFromString(std::string_view s) {
    if (s == "Fiction") return Genre::Fiction;
    if (s == "NonFiction") return Genre::NonFiction;
    if (s == "SciFi") return Genre::SciFi;
    if (s == "Biography") return Genre::Biography;
    if (s == "Mystery") return Genre::Mystery;
    return Genre::Unknown;
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string_view author;
    std::string title;

    int year;
    Genre genre;
    double rating;
    int read_count;
    auto operator<=>(const Book&) const = default;
    // Ваш код для конструкторов здесь
    // db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    constexpr Book(std::string t, std::string_view a, int y, Genre g, double rating, int read_count):
        title(t), author(a), year(y), genre(g), rating(rating), read_count(read_count) {}
    constexpr Book(std::string t, std::string_view a, int y, const std::string& g, double rating, int read_count):
        Book(std::move(t), a, y, GenreFromString(g), rating, read_count) {}
};
}  // namespace bookdb

namespace std {

template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};


template <>
struct formatter<bookdb::Book> {
    constexpr auto parse(format_parse_context& ctx) {
        return ctx.begin();
    }
    
    auto format(const bookdb::Book& book, format_context& ctx) const {
        // Используем явные индексы для избежания ошибки
        return format_to(
            ctx.out(),
            "{0}: {1} ({2}, {3}, rating {4}, reads {5})",
            book.author,
            book.title,
            book.year,
            book.genre,
            book.rating,
            book.read_count
        );
    }
};

}  // namespace std
