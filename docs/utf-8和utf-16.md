```cpp
inline int hexToInt(char c) {
    if ('0' <= c && c <= '9') return c - '0';
    if ('a' <= c && c <= 'f') return c - 'a' + 10;
    if ('A' <= c && c <= 'F') return c - 'A' + 10;
    throw std::runtime_error("Invalid hex digit");
}

inline char intToHex(int i) {
    if (0 <= i && i <= 9) return i + '0';
    if (10 <= i && i <= 16) return i + 'A' - 10;
    throw std::runtime_error("Invalid hex digit");
}

inline uint32_t parseHex4(std::string_view sv, size_t pos) {
    if (pos + 3 >= sv.size()) throw std::runtime_error("Incomplete unicode escape");
    uint32_t val {};
    for (int i = 0; i < 4; ++i) {
        val = (val << 4) | hexToInt(sv[pos + i]);
    }
    return val;
}

inline void appendUnicodeHex4(std::string& s, uint32_t hex4) {
    s += "\\u";
    for (int i = 0; i < 4; i++) {
        s.push_back(intToHex((hex4 >> (12 - (i << 2))) & 0xF));
    }
}

/*
    cp 即 code point -> 码点

    U+0000  ~ U+007F (ASCII)   单字节 字节格式: 0xxxxxxx
    U+0080  ~ U+07FF           双字节 字节格式: 110xxxxx 10xxxxxx
    U+0800  ~ U+FFFF           三字节 字节格式: 1110xxxx 10xxxxxx 10xxxxxx
    U+10000 ~ U+10FFFF (Emoji) 四字节 字节格式: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
*/
inline void appendUtf8(std::string& s, uint32_t cp) {
    if (cp <= 0x7F) {
        s.push_back(static_cast<char>(cp));
    }
    else if (cp <= 0x7FF) {
        // U+0080 ~ U+07FF 是 0000 10000000 ~ 0111 11111111
        // cp >> 6 取前 5 位, 0x1F 是 0001 1111 作为掩码; 0xC0 是 1100 0000, 这里的 110 前缀表示为 2 字节字符
        // 后面的 0x80 是 1000 0000 取的 10 前缀, 0x3F 是 0011 1111 掩码
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
};

inline void appendEscapeUnicode(std::string& s, std::string_view utf8_s, size_t pos, size_t unicode_size) {
    // 假设 pos 是 utf8_s 表示 unicode 第一个字符
    if (unicode_size == 0) return;

    using bt = unsigned char;

    // 基本平面
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

inline size_t isUnicodePrefix(char prefix_c) {
    uint32_t b = prefix_c;
    if ((b & 0x80) == 0x00) return 1;
    if ((b & 0xE0) == 0xC0) return 2;
    if ((b & 0xF0) == 0xE0) return 3;
    if ((b & 0xF8) == 0xF0) return 4;
    return 0;
}

inline std::string flattenedString(std::string_view quoted_str) {
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
                /*
                    JSON 采用 UTF-16, 以 2 字节, 也就是 \uxxxx 为一个基本单位
                    这里存储了 U+0000 ~ U+FFFF 的所有码点, 称为基本平面. 直接取出 xxxx 16进制数然后转换成对应 unicode 即可
                    
                    对于 Emoji 等 (U+10000 以上) 码点需要 3 个以上字节来装了
                    则取 2 个基本单位, 也就是 4 字节 \uxxxx\uxxxx 作为一个代理对。这称为代理平面

                    unicode 预留了 0xD800 (1101 1000 0000 0000) 到 0xDFFF (1101 1111 1111 1111) 
                    这里有 11 位我们能用, 但是 utf-8 四字节要求 2^21-1 种状态 用 20 位 bit 就可以表示
                    于是把多的那 1 位用来区分 前导 与 后继 

                    那么我们将 0xD800 (1101 1000 0000 0000) ~ 0xDBFF (1101 1011 1111 1111) 作为前导, 这里可用 10 位
                    0xDC00 (1101 1100 0000 0000) ~ 0xDFFF (1101 1111 1111 1111) 作为后继, 这里可用 10 位

                    拼合, 再加上基数 0x10000, 就得到了四字节的所有范围了.
                    计算: cp = 0x10000 + ((high - 0xD800) << 10) + (low - 0xDC00)
                */
                // 满足了前导的需求范围
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

inline std::string quotedString(std::string_view raw_str) {
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

inline std::string quotedStringToJson(std::string_view raw_str) {
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
```