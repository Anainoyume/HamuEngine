#include "JsonLexer.h"

namespace hamu::utils
{

    void JsonLexer::setInput(std::string_view json) noexcept {
        json_      = json;
        cursor_    = 0;
        end_       = json.size();
        line_      = 1;
        column_    = 1;
        has_cache_ = false;
    }

    JsonToken JsonLexer::Next() {
        if (json_.empty()) throw std::runtime_error("JsonLexer: Empty JSON input.");
        if (cursor_ >= end_) {
            JsonToken token;
            token.type = JsonTokenType::End;
            return token;
        }

        if (has_cache_) {
            auto size  = peek_cache_.content.size();
            has_cache_ = false;
            cursor_ += size;
            column_ += size;
            return peek_cache_;
        }

        auto token = has_cache_ ? peek_cache_ : Peek(false);
        auto size  = token.content.size();
        cursor_ += size;
        column_ += size;
        return token;
    }

    JsonToken JsonLexer::Peek(bool cache) {
        if (json_.empty()) throw std::runtime_error("JsonLexer: Empty JSON input.");
        if (has_cache_) return peek_cache_;

        removeSpace();

        // clang-format off
        char c = json_[cursor_];

        if (!cache) {
            switch (c) {
                case '{': return JsonToken {json_.substr(cursor_, 1), JsonTokenType::LeftBrace, line_, column_};    
                case '}': return JsonToken {json_.substr(cursor_, 1), JsonTokenType::RightBrace, line_, column_};   
                case '[': return JsonToken {json_.substr(cursor_, 1), JsonTokenType::LeftBracket, line_, column_};  
                case ']': return JsonToken {json_.substr(cursor_, 1), JsonTokenType::RightBracket, line_, column_}; 
                case ':': return JsonToken {json_.substr(cursor_, 1), JsonTokenType::Colon, line_, column_};        
                case ',': return JsonToken {json_.substr(cursor_, 1), JsonTokenType::Comma, line_, column_};        
                case '"': return lexString();                                                                       
                case '-': return lexNumber();                                                                       
                case 't': return lexTrue();                                                                         
                case 'f': return lexFalse();                                                                        
                case 'n': return lexNull();                                                                         
            }
            if ('0' <= c && c <= '9') return lexNumber(); 
            return errorToken();
        }

        has_cache_ = true;
        switch (c) {
            case '{': peek_cache_ = JsonToken {json_.substr(cursor_, 1), JsonTokenType::LeftBrace, line_, column_};    return peek_cache_;
            case '}': peek_cache_ = JsonToken {json_.substr(cursor_, 1), JsonTokenType::RightBrace, line_, column_};   return peek_cache_;
            case '[': peek_cache_ = JsonToken {json_.substr(cursor_, 1), JsonTokenType::LeftBracket, line_, column_};  return peek_cache_;
            case ']': peek_cache_ = JsonToken {json_.substr(cursor_, 1), JsonTokenType::RightBracket, line_, column_}; return peek_cache_;
            case ':': peek_cache_ = JsonToken {json_.substr(cursor_, 1), JsonTokenType::Colon, line_, column_};        return peek_cache_;
            case ',': peek_cache_ = JsonToken {json_.substr(cursor_, 1), JsonTokenType::Comma, line_, column_};        return peek_cache_;
            case '"': peek_cache_ = lexString();                                                                       return peek_cache_;
            case '-': peek_cache_ = lexNumber();                                                                       return peek_cache_;
            case 't': peek_cache_ = lexTrue();                                                                         return peek_cache_;
            case 'f': peek_cache_ = lexFalse();                                                                        return peek_cache_;
            case 'n': peek_cache_ = lexNull();                                                                         return peek_cache_;
        }
        if ('0' <= c && c <= '9') { peek_cache_ = lexNumber(); return peek_cache_; }
        // clang-format on

        // 非法字符
        return errorToken();
    }

    void JsonLexer::removeSpace() noexcept {
        if (cursor_ >= end_) return;
        while (isSpace(json_[cursor_])) {
            if (json_[cursor_] == '\n') {
                column_ = 0;
                line_++;
            }
            column_++;
            cursor_++;
        };
    }

    std::string_view JsonToken::typeToString(JsonTokenType type) noexcept {
        switch (type) {
            case JsonTokenType::LeftBrace: return "LeftBrace";
            case JsonTokenType::RightBrace: return "RightBrace";
            case JsonTokenType::LeftBracket: return "LeftBracket";
            case JsonTokenType::RightBracket: return "RightBracket";
            case JsonTokenType::Colon: return "Colon";
            case JsonTokenType::Comma: return "Comma";
            case JsonTokenType::String: return "String";
            case JsonTokenType::Number: return "Number";
            case JsonTokenType::True: return "True";
            case JsonTokenType::False: return "False";
            case JsonTokenType::Null: return "Null";
            case JsonTokenType::Comment: return "Comment";
            case JsonTokenType::Identifier: return "Identifier";
            case JsonTokenType::Empty: return "Empty";
            case JsonTokenType::End: return "End";
            case JsonTokenType::Error: return "Error";
            default: return "unknown";
        }
    }

    std::ostream& operator<<(std::ostream& os, const JsonToken& token) {
        if (token.type == JsonTokenType::Error) {
            os << "[Error] " << "(line: " << token.line << ", column: " << token.column << ")";
            return os;
        }
        os << "[" << JsonToken::typeToString(token.type) << "]  " << token.content;
        return os;
    }

    // --------------------------------- Private -----------------------------------

    JsonToken JsonLexer::lexString() const {
        // 第一个字符是左引号
        auto begin = cursor_;
        for (auto i = begin + 1; i < end_; i++) {
            auto c = json_[i];
            if (c == '\n') {
                return errorToken();
            }
            if (c == '\\') {
                i++;
                continue;
            }
            if (c == '"') {
                return JsonToken {
                    json_.substr(begin, i - begin + 1),
                    JsonTokenType::String,
                    line_,
                    column_,
                };
            }
        }
        // 字符串未闭合
        return errorToken();
    }

    JsonToken JsonLexer::lexNumber() const {
        auto state         = NumberState::Digit;
        auto begin         = cursor_;
        bool leadingZero   = false;
        bool fractionStage = false;
        bool exponentStage = false;

        // Begin State
        switch (json_[begin]) {
            case '-': state = NumberState::Sign; break;
            case '0': leadingZero = true; break;
        }

        for (auto i = begin + 1; i < end_; i++) {
            auto c = json_[i];

            // 判断终止
            if (isTokenTerminator(c)) {
                if (!isDigit(json_[i - 1])) return errorToken(); // 非数字结尾
                return JsonToken {json_.substr(begin, i - begin), JsonTokenType::Number, line_, column_};
            }

            switch (state) {
                case NumberState::Sign: {
                    if (!isDigit(c)) return errorToken();
                    state = NumberState::Digit;
                    if (c == '0' && !exponentStage) {
                        leadingZero = true;
                    }
                    break;
                }

                case NumberState::Digit: {
                    if (!fractionStage && c == '.') {
                        fractionStage = true;
                        leadingZero   = false;
                        state         = NumberState::Point;
                        break;
                    }
                    if (!exponentStage && (c == 'e' || c == 'E')) {
                        fractionStage = true;
                        exponentStage = true;
                        leadingZero   = false;
                        state         = NumberState::Exponent;
                        break;
                    }
                    if (leadingZero) return errorToken();
                    if (isDigit(c)) break;
                    return errorToken();
                }

                case NumberState::Point: {
                    if (isDigit(c)) {
                        state = NumberState::Digit;
                        break;
                    }
                    return errorToken();
                }

                case NumberState::Exponent: {
                    if (c == '-' || c == '+') {
                        state = NumberState::Sign;
                        break;
                    }
                    if (isDigit(c)) {
                        state = NumberState::Digit;
                        break;
                    }
                    return errorToken();
                }
            }
        }
        return errorToken();
    }

    JsonToken JsonLexer::lexTrue() const {
        auto begin = cursor_;
        if (begin + 4 >= end_ || !isTokenTerminator(json_[begin + 4]) || json_.substr(begin, 4) != "true") {
            return errorToken();
        }
        return JsonToken {json_.substr(begin, 4), JsonTokenType::True, line_, column_};
    }

    JsonToken JsonLexer::lexFalse() const {
        auto begin = cursor_;
        if (begin + 5 >= end_ || !isTokenTerminator(json_[begin + 5]) || json_.substr(begin, 5) != "false") {
            return errorToken();
        }
        return JsonToken {json_.substr(begin, 5), JsonTokenType::False, line_, column_};
    }

    JsonToken JsonLexer::lexNull() const {
        auto begin = cursor_;
        if (begin + 4 >= end_ || !isTokenTerminator(json_[begin + 4]) || json_.substr(begin, 4) != "null") {
            return errorToken();
        }
        return JsonToken {json_.substr(begin, 4), JsonTokenType::Null, line_, column_};
    }

} // namespace hamu::utils