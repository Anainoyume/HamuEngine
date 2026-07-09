#include "JsonUtils.h"

#include "tiny_json/JsonDOMBuilder.h"
#include "tiny_json/JsonParser.h"
#include "utils/Utils.h"

#include <cstddef>
#include <cstdint>
#include <format>
#include <stdexcept>
#include <string>

namespace hamu::utils
{
    int hexToInt(char c) {
        if ('0' <= c && c <= '9') return c - '0';
        if ('a' <= c && c <= 'f') return c - 'a' + 10;
        if ('A' <= c && c <= 'F') return c - 'A' + 10;
        throw std::runtime_error("Invalid hex digit");
    }

    char intToHex(int i) {
        if (0 <= i && i <= 9) return i + '0';
        if (10 <= i && i <= 16) return i + 'A' - 10;
        throw std::runtime_error("Invalid hex digit");
    }

    uint32_t parseHex4(std::string_view sv, size_t pos) {
        if (pos + 3 >= sv.size()) throw std::runtime_error("Incomplete unicode escape");
        uint32_t val {};
        for (int i = 0; i < 4; ++i) {
            val = (val << 4) | hexToInt(sv[pos + i]);
        }
        return val;
    }

    void appendUnicodeHex4(std::string& s, uint32_t hex4) {
        s += "\\u";
        for (int i = 0; i < 4; i++) {
            s.push_back(intToHex((hex4 >> (12 - (i << 2))) & 0xF));
        }
    }

    void appendUtf8(std::string& s, uint32_t cp) {
        if (cp <= 0x7F) {
            s.push_back(static_cast<char>(cp));
        }
        else if (cp <= 0x7FF) {
            s.push_back(static_cast<char>(0xC0 | ((cp >> 6) & 0x1F)));
            s.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0xFFFF) {
            // 之后的逻辑都是大同小异
            s.push_back(static_cast<char>(0xE0 | ((cp >> 12) & 0x0F)));
            s.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            s.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else if (cp <= 0x10FFFF) {
            s.push_back(static_cast<char>(0xF0 | ((cp >> 18) & 0x07)));
            s.push_back(static_cast<char>(0x80 | ((cp >> 12) & 0x3F)));
            s.push_back(static_cast<char>(0x80 | ((cp >> 6) & 0x3F)));
            s.push_back(static_cast<char>(0x80 | (cp & 0x3F)));
        }
        else {
            s.push_back('?'); // 无效字符替换
        }
    }

    void appendEscapeUnicode(std::string& s, std::string_view utf8_s, size_t pos, size_t unicode_size) {
        if (unicode_size == 0) return;

        using bt = unsigned char;

        size_t size = utf8_s.size();
        uint32_t cp {};
        auto b1 = static_cast<bt>(utf8_s[pos]);
        auto b2 = pos + 1 < size ? static_cast<bt>(utf8_s[pos + 1]) : 0;
        auto b3 = pos + 2 < size ? static_cast<bt>(utf8_s[pos + 2]) : 0;
        auto b4 = pos + 3 < size ? static_cast<bt>(utf8_s[pos + 3]) : 0;

        if (unicode_size == 1) cp = b1 & 0x7F;
        if (unicode_size == 2) cp = ((b1 & 0x1F) << 6) + (b2 & 0x3F);
        if (unicode_size == 3) cp = ((b1 & 0xF) << 12) + ((b2 & 0x3F) << 6) + (b3 & 0x3F);
        if (unicode_size == 4) {
            cp            = ((b1 & 0x7) << 18) + ((b2 & 0x3F) << 12) + ((b3 & 0x3F) << 6) + (b4 & 0x3F) - 0x10000;
            uint32_t high = ((cp >> 10) & 0x3FF) + 0xD800;
            uint32_t low  = (cp & 0x3FF) + 0xDC00;
            appendUnicodeHex4(s, high);
            appendUnicodeHex4(s, low);
            return;
        }
        appendUnicodeHex4(s, cp);
    }

    size_t isUnicodePrefix(char prefix_c) {
        uint32_t b = prefix_c;
        if ((b & 0x80) == 0x00) return 1;
        if ((b & 0xE0) == 0xC0) return 2;
        if ((b & 0xF0) == 0xE0) return 3;
        if ((b & 0xF8) == 0xF0) return 4;
        return 0;
    }

    std::string flattenedString(std::string_view quoted_str) {
        std::string raw_str;
        size_t size = quoted_str.size();
        raw_str.reserve(size);

        for (size_t i = 0; i < size; i++) {
            auto c = quoted_str[i];

            if (c != '\\') {
                raw_str.push_back(c);
                continue;
            }
            if (i + 1 >= size) throw std::runtime_error("Unexpected end of string");

            // clang-format off
            switch (quoted_str[i+1]) {
                case '"' : raw_str.push_back('"');  break;
                case '\\': raw_str.push_back('\\'); break;
                case '/' : raw_str.push_back('/');  break;
                case 'b' : raw_str.push_back('\b'); break;
                case 'f' : raw_str.push_back('\f'); break;
                case 'n' : raw_str.push_back('\n'); break;
                case 'r' : raw_str.push_back('\r'); break;
                case 't' : raw_str.push_back('\t'); break;
                case 'u' : {
                    if (i + 5 >= size) throw std::runtime_error("Invalid unicode escape sequence length");
                    
                    uint32_t cp = parseHex4(quoted_str, i+2); 
                    i += 5;
                    if (0xD800 <= cp && cp <= 0xDBFF) {
                        if (i + 2 < size && quoted_str[i+1] == '\\' && quoted_str[i+2] == 'u') {
                            size_t temp_idx = i + 1; 
                            uint32_t low_cp = parseHex4(quoted_str, temp_idx+2);
                            temp_idx += 5;
                            
                            if (low_cp >= 0xDC00 && low_cp <= 0xDFFF) {
                                cp = 0x10000 + ((cp - 0xD800) << 10) + (low_cp - 0xDC00);
                                i = temp_idx;
                            }
                        }
                    }
                    appendUtf8(raw_str, cp);
                    i -= 1;
                    break;
                }
                default: throw std::runtime_error("Invalid escape sequence");
            }
            // clang-format on
            i += 1;
        }
        return raw_str;
    }

    std::string quotedString(std::string_view raw_str) {
        std::string quoted_str;
        size_t size = raw_str.size();
        quoted_str.reserve(size * 1.5);

        for (size_t i = 0; i < size; i++) {
            char c = raw_str[i];
            // clang-format off
            switch (c) {
                case '\"': quoted_str += "\\\""; break;
                case '\\': quoted_str += "\\\\"; break;
                case '\b': quoted_str += "\\b";  break;
                case '\f': quoted_str += "\\f";  break;
                case '\n': quoted_str += "\\n";  break;
                case '\r': quoted_str += "\\r";  break;
                case '\t': quoted_str += "\\t";  break;
                default: {
                    if (auto count = isUnicodePrefix(c); count > 0) {
                        if (count == 1 && static_cast<unsigned char>(c) >= 0x20) {
                            quoted_str.push_back(c);
                            break;
                        }
                        appendEscapeUnicode(quoted_str, raw_str, i, count);
                        i += count - 1;
                        break;
                    }
                    quoted_str.push_back(c);
                    break;
                }
            }
            // clang-format on
        }
        return quoted_str;
    }

    std::string quotedStringToJson(std::string_view raw_str) {
        std::string quoted_str;
        size_t size = raw_str.size();
        quoted_str.reserve(size * 1.5);

        for (size_t i = 0; i < size; i++) {
            char c = raw_str[i];
            // clang-format off
            switch (c) {
                case '\"': quoted_str += "\\\""; break;
                case '\\': quoted_str += "\\\\"; break;
                case '\b': quoted_str += "\\b";  break;
                case '\f': quoted_str += "\\f";  break;
                case '\n': quoted_str += "\\n";  break;
                case '\r': quoted_str += "\\r";  break;
                case '\t': quoted_str += "\\t";  break;
                default: {
                    if (auto count = isUnicodePrefix(c); count == 1 && static_cast<unsigned char>(c) < 0x20) {
                        appendEscapeUnicode(quoted_str, raw_str, i, count);
                        break;
                    }
                    quoted_str.push_back(c);
                    break;
                }
            }
            // clang-format on
        }
        return quoted_str;
    }

    std::expected<JsonNode, std::string> ParserJson(std::string_view json) {
        JsonDOMBuilder builder;
        JsonParser parser(builder, json);

        auto result = parser.parser();
        if (result.error()) {
            std::string error_msg {result.info};
            error_msg += std::format(" (line: {}, column: {})", result.line, result.column);
            return std::unexpected(std::move(error_msg));
        }

        return builder.getRoot();
    }

    /**
     * @brief 解析指定 path 的 json 文件, 返回 std::expected<JsonNode, std::string>
     * 
     * @param path json 文件路径
     * @return std::expected<JsonNode, std::string> 如果正确返回 JsonNode, 否则 std::string 为错误信息 
     */
    std::expected<JsonNode, std::string> ParserJson(fs::path path) {
        auto json = ReadText(path);
        return ParserJson(std::string_view {json});
    }

} // namespace hamu::utils