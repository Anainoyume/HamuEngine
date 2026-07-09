#pragma once

#include "JsonNode.h"

#include <expected>
#include <filesystem>
namespace fs = std::filesystem;

// clang-format off
#define HAMU_INTERNAL_ARGS_N(                                                         \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16,            \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32,   \
    _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48,   \
    _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64,   \
    N, ...) N

#define HAMU_GET_ARGS_COUNT(...) HAMU_INTERNAL_ARGS_N(__VA_ARGS__,  \
    64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50,     \
    49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35,     \
    34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20,     \
    19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,      \
    4,  3,  2,  1,  0)
// clang-format on

#define HAMU_CONCAT(a, b) a##b
#define HAMU_CONCAT_EXPAND(a, b) HAMU_CONCAT(a, b)

namespace hamu::utils
{
    int hexToInt(char c);
    uint32_t parseHex4(std::string_view sv, size_t pos);
    void appendUtf8(std::string& s, uint32_t cp);
    std::string flattenedString(std::string_view quoted_str);

    char intToHex(int i);
    size_t isUnicodePrefix(char prefix_c);
    void appendUnicodeHex4(std::string& s, uint32_t hex4);
    void appendEscapeUnicode(std::string& s, std::string_view utf8_s, size_t pos, size_t unicode_size);
    std::string quotedString(std::string_view raw_str);
    std::string quotedStringToJson(std::string_view raw_str);

    std::expected<JsonNode, std::string> ParserJson(std::string_view json);
    std::expected<JsonNode, std::string> ParserJson(fs::path path);

} // namespace hamu::utils