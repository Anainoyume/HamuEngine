#pragma once

#include "JsonLexer.h"
#include <cstddef>
#include <ostream>
#include <string_view>

namespace hamu::utils
{

    template <typename T>
    concept JsonHandleType = requires(T& derived, bool val, std::string_view sv) {
        { derived.OnNull() } -> std::same_as<void>;
        { derived.OnBool(val) } -> std::same_as<void>;

        { derived.OnNumber(sv) } -> std::same_as<void>;
        { derived.OnString(sv) } -> std::same_as<void>;
        { derived.OnKey(sv) } -> std::same_as<void>;

        { derived.OnBeginObject() } -> std::same_as<void>;
        { derived.OnEndObject() } -> std::same_as<void>;
        { derived.OnBeginArray() } -> std::same_as<void>;
        { derived.OnEndArray() } -> std::same_as<void>;
    };

    enum class JsonParserState
    {
        Ok,
        Error
    };

    struct JsonParserResult
    {
        std::string_view info;
        size_t line           = 0;
        size_t column         = 0;
        JsonParserState state = JsonParserState::Ok;

        bool ok() const noexcept { return state == JsonParserState::Ok; }
        bool error() const noexcept { return state == JsonParserState::Error; }

        friend std::ostream& operator<<(std::ostream& os, const JsonParserResult& result);
    };

    class DepthScope
    {
    public:
        DepthScope(size_t& depth) : depth_(depth) { depth_ += 1; }
        ~DepthScope() { depth_ -= 1; }

    private:
        size_t& depth_;
    };

    template <JsonHandleType Handle>
    class JsonParser final
    {
    public:
        ~JsonParser() = default;

        explicit JsonParser(Handle& handle) : handle_(handle) {}
        explicit JsonParser(Handle& handle, std::string_view json) : lexer_(json), handle_(handle) {}

        JsonParser(const JsonParser&)            = delete;
        JsonParser& operator=(const JsonParser&) = delete;
        JsonParser(JsonParser&&)                 = delete;
        JsonParser& operator=(JsonParser&&)      = delete;

        JsonParserResult parser();
        JsonParserResult parser(std::string_view json);

        inline static size_t MAX_PARSER_DEPTH = 100;

    private:
        JsonLexer lexer_;
        Handle& handle_;

        size_t depth_ = 0;

        JsonParserResult result_Ok() const noexcept { return JsonParserResult {}; }
        JsonParserResult result_Error(std::string_view info, size_t line, size_t column) const noexcept {
            JsonParserResult result;
            result.state  = JsonParserState::Error;
            result.info   = info;
            result.line   = line;
            result.column = column;
            return result;
        }

        JsonParserResult parserObject();
        JsonParserResult parserArray();
    };

} // namespace hamu::utils

#include "JsonParser.tpp"