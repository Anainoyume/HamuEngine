#pragma once

#include "JsonParser.h"

namespace hamu::utils
{

    template <JsonHandleType Handle>
    inline JsonParserResult JsonParser<Handle>::parser() {
        depth_ = 0;

        auto begin_token = lexer_.Next();
        if (begin_token.type == JsonTokenType::LeftBrace) return parserObject();
        if (begin_token.type == JsonTokenType::LeftBracket) return parserArray();

        // 其余情况都是非法的
        return result_Error("JsonParser: 以非 '{' 或 '[' 开头.", begin_token.line, begin_token.column);
    }


    template <JsonHandleType Handle>
    inline JsonParserResult JsonParser<Handle>::parser(std::string_view json) {
        lexer_.setInput(json);
        return parser();
    }


    template <JsonHandleType Handle>
    inline JsonParserResult JsonParser<Handle>::parserObject() {
        auto token = lexer_.Next();
        DepthScope _(depth_);
        if (depth_ > MAX_PARSER_DEPTH) {
            return result_Error("JsonParser: 超过了设置的递归深度.", token.line, token.column);
        }

        // Object 开始
        handle_.OnBeginObject();

        // 特殊情况: 立刻闭合
        if (!token.isValid()) result_Error("JsonParser: Object 结构不正确.", token.line, token.column);
        if (token.type == JsonTokenType::RightBrace) {
            handle_.OnEndObject();
            return result_Ok();
        }

        while (token.isValid()) {
            // key
            if (token.type != JsonTokenType::String) {
                return result_Error("JsonParser: 需要键.", token.line, token.column);
            }
            handle_.OnKey(token.content.substr(1, token.content.size() - 2));

            // 冒号结构
            if (token = lexer_.Next(); token.type != JsonTokenType::Colon) {
                return result_Error("JsonParser: 键后面缺少 ':'.", token.line, token.column);
            }

            // 字面量
            token = lexer_.Next();
            JsonParserResult result;
            if (token.isLiterals()) {
                // clang-format off
            switch (token.type) {
                case JsonTokenType::String: handle_.OnString(token.content.substr(1, token.content.size() - 2)); break;
                case JsonTokenType::Number: handle_.OnNumber(token.content);                                     break;
                case JsonTokenType::True:   handle_.OnBool(true);                                                break;
                case JsonTokenType::False:  handle_.OnBool(false);                                               break;
                case JsonTokenType::Null:   handle_.OnNull();                                                    break;
                default: return result_Error("JsonParser: 非法字面量.", token.line, token.column);
            }
                // clang-format on
            }
            else if (token.type == JsonTokenType::LeftBrace) {
                result = parserObject();
            }
            else if (token.type == JsonTokenType::LeftBracket) {
                result = parserArray();
            }
            if (result.error()) return result;

            // 逗号
            token = lexer_.Next();
            if (token.type == JsonTokenType::Comma) {
                token = lexer_.Next();
                continue;
            }

            // 闭合
            if (token.type == JsonTokenType::RightBrace) {
                handle_.OnEndObject();
                return result_Ok();
            }

            // 非法字符
            return result_Error("JsonParser: Object 结构不正确.", token.line, token.column);
        }
        return result_Error("JsonParser: Object 结构不正确.", token.line, token.column);
    }


    template <JsonHandleType Handle>
    inline JsonParserResult JsonParser<Handle>::parserArray() {
        auto token = lexer_.Next();
        DepthScope _(depth_);
        if (depth_ > MAX_PARSER_DEPTH) {
            return result_Error("JsonParser: 超过了设置的递归深度.", token.line, token.column);
        }

        // Array 开始
        handle_.OnBeginArray();

        // 特殊情况: 立刻闭合
        if (!token.isValid()) result_Error("JsonParser: Array 结构不正确.", token.line, token.column);
        if (token.type == JsonTokenType::RightBracket) {
            handle_.OnEndArray();
            return result_Ok();
        }

        while (token.isValid()) {
            // 字面量
            JsonParserResult result;
            if (token.isLiterals()) {
                // clang-format off
            switch (token.type) {
                case JsonTokenType::String: handle_.OnString(token.content.substr(1, token.content.size() - 2)); break;
                case JsonTokenType::Number: handle_.OnNumber(token.content);                                     break;
                case JsonTokenType::True:   handle_.OnBool(true);                                                break;
                case JsonTokenType::False:  handle_.OnBool(false);                                               break;
                case JsonTokenType::Null:   handle_.OnNull();                                                    break;
                default: return result_Error("JsonParser: 非法字面量.", token.line, token.column);
            }
                // clang-format on
            }
            else if (token.type == JsonTokenType::LeftBrace) {
                result = parserObject();
            }
            else if (token.type == JsonTokenType::LeftBracket) {
                result = parserArray();
            }
            if (result.error()) return result;

            // 逗号
            token = lexer_.Next();
            if (token.type == JsonTokenType::Comma) {
                token = lexer_.Next();
                continue;
            }

            // 闭合
            if (token.type == JsonTokenType::RightBracket) {
                handle_.OnEndArray();
                return result_Ok();
            }

            // 非法字符
            return result_Error("JsonParser: Array 结构不正确.", token.line, token.column);
        }
        return result_Error("JsonParser: Array 结构不正确.", token.line, token.column);
    }

} // namespace hamu::utils