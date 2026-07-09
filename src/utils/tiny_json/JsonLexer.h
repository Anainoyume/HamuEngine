#pragma once

#include <cctype>
#include <ostream>
#include <string_view>

namespace hamu::utils
{
    enum class JsonTokenType
    {
        // Structural (结构符)
        LeftBrace,    // {
        RightBrace,   // }
        LeftBracket,  // [
        RightBracket, // ]
        Colon,        // :
        Comma,        // ,

        // Literals (字面量)
        String,
        Number,
        True,
        False,
        Null,

        // Future Proof (扩展)
        Comment,    // 注释
        Identifier, // 不带引号的 Key

        // Control (控制符)
        Empty,
        End,
        Error // 非法字符
    };

    // 语素
    struct JsonToken
    {
        std::string_view content;
        JsonTokenType type = JsonTokenType::Empty;
        size_t line        = 0;
        size_t column      = 0;

        bool isEmpty() const noexcept { return type == JsonTokenType::Empty; }
        bool isEnd() const noexcept { return type == JsonTokenType::End; }
        bool isError() const noexcept { return type == JsonTokenType::Error; }
        bool isValid() const noexcept { return type != JsonTokenType::End && type != JsonTokenType::Error; }
        bool isLiterals() const noexcept {
            return type == JsonTokenType::String || type == JsonTokenType::Number || type == JsonTokenType::True ||
                   type == JsonTokenType::False || type == JsonTokenType::Null;
        }

        static std::string_view typeToString(JsonTokenType type) noexcept;
        friend std::ostream& operator<<(std::ostream& os, const JsonToken& token);
    };

    class JsonLexer final
    {
    public:
        ~JsonLexer()         = default;
        explicit JsonLexer() = default;
        explicit JsonLexer(std::string_view json) noexcept : json_(json), end_(json.size()) {}

        JsonLexer(const JsonLexer&)            = delete;
        JsonLexer& operator=(const JsonLexer&) = delete;
        JsonLexer(JsonLexer&&)                 = delete;
        JsonLexer& operator=(JsonLexer&&)      = delete;

        void setInput(std::string_view json) noexcept;
        JsonToken Next();
        JsonToken Peek(bool cache = true);

    private:
        std::string_view json_;
        size_t cursor_ = 0;
        size_t end_    = 0;
        size_t line_   = 1;
        size_t column_ = 1;

        JsonToken peek_cache_;
        bool has_cache_ = false;

        bool isSpace(char c) const noexcept { return std::isspace(c); }
        bool isDigit(char c) const noexcept { return '0' <= c && c <= '9'; }
        bool isTokenTerminator(char c) const noexcept { return isSpace(c) || c == '}' || c == ']' || c == ','; }
        void removeSpace() noexcept;

        JsonToken errorToken() const noexcept {
            JsonToken error;
            error.type   = JsonTokenType::Error;
            error.line   = line_;
            error.column = column_;
            return error;
        }

        JsonToken errorToken(size_t line, size_t column) const noexcept {
            JsonToken error;
            error.type   = JsonTokenType::Error;
            error.line   = line;
            error.column = column;
            return error;
        }

        JsonToken lexString() const;
        JsonToken lexNumber() const;
        JsonToken lexTrue() const;
        JsonToken lexFalse() const;
        JsonToken lexNull() const;

        enum class NumberState
        {
            Sign,
            Digit,
            Point,
            Exponent
        };
    };

} // namespace hamu::utils