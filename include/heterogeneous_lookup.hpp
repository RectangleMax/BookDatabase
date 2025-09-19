#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void; 

    bool operator()(const std::string& a, const std::string& b) const {
        return a < b;
    }

    bool operator()(const std::string& a, std::string_view b) const {
        return a < b;
    }
    bool operator()(std::string_view a, const std::string& b) const {
        return a < b;
    }

    bool operator()(const std::string& a, const char* b) const {
        return a < b; 
    }

    bool operator()(const char* a, const std::string& b) const {
        return std::string_view(a) < b; 
    }
};

struct TransparentStringEqual {
    using is_transparent = void;

    bool operator()(const std::string& a, const std::string& b) const {
        return a == b;
    }

    bool operator()(const std::string& a, const char* b) const {
        return a == b;  // a == std::string_view(b);
    }

    bool operator()(const std::string& a, std::string_view b) const {
        return a == b;
    }
};

struct TransparentStringHash {
    using is_transparent = void; // Разрешает гетерогенные ключи

    size_t operator()(const std::string& s) const {
        return std::hash<std::string>{}(s);
    }

    size_t operator()(const char* s) const {
        return std::hash<std::string_view>{}(s);
    }

    size_t operator()(std::string_view s) const {
        return std::hash<std::string_view>{}(s);
    }
};

}  // namespace bookdb
