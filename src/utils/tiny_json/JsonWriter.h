#pragma once

#include "JsonUtils.h"

#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>

namespace hamu::utils
{
    template <typename T>
    concept JsonNumber = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

    class JsonWriter
    {
    public:
        explicit JsonWriter(std::string& buffer) noexcept : buffer_(buffer) {}

        void raw(char c) { buffer_.push_back(c); }
        void writeNull() { buffer_ += "null"; }
        void writeBool(bool b) { buffer_ += (b ? "true" : "false"); }

        void writeString(std::string_view str) {
            buffer_.push_back('"');
            buffer_ += quotedStringToJson(str);
            buffer_.push_back('"');
        }

        void writeKey(std::string_view key) {
            writeString(key);
            buffer_.push_back(':');
        }

        template <JsonNumber T>
        void writeNumber(T number) {
            char buf[64];
            auto [ptr, ec] = std::to_chars(buf, buf + sizeof(buf), number);
            if (ec == std::errc {}) {
                buffer_.append(buf, ptr - buf);
            }
            else {
                buffer_.push_back('0');
            }
        }

    private:
        std::string& buffer_;
    };

} // namespace hamu::utils