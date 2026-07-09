#pragma once

#include "JsonDOMBuilder.h"
#include "JsonParser.h"
#include "JsonWriter.h"
#include "JsonUtils.h"
#include "runtime/tool/Utils.h"

#include <cstddef>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string_view>
#include <variant>
#include <vector>

#define HAMU_JSON_FIELD_SERDE(Field) o.field(#Field, val.Field);
#define HAMU_JSON_FIELD_DESERDE(Field) deserde(root[#Field], object.Field);

#define HAMU_FOR_EACH_0(M)
#define HAMU_FOR_EACH_1(M, i) M(i)
#define HAMU_FOR_EACH_2(M, i, ...) M(i) HAMU_FOR_EACH_1(M, __VA_ARGS__)
#define HAMU_FOR_EACH_3(M, i, ...) M(i) HAMU_FOR_EACH_2(M, __VA_ARGS__)
#define HAMU_FOR_EACH_4(M, i, ...) M(i) HAMU_FOR_EACH_3(M, __VA_ARGS__)
#define HAMU_FOR_EACH_5(M, i, ...) M(i) HAMU_FOR_EACH_4(M, __VA_ARGS__)
#define HAMU_FOR_EACH_6(M, i, ...) M(i) HAMU_FOR_EACH_5(M, __VA_ARGS__)
#define HAMU_FOR_EACH_7(M, i, ...) M(i) HAMU_FOR_EACH_6(M, __VA_ARGS__)
#define HAMU_FOR_EACH_8(M, i, ...) M(i) HAMU_FOR_EACH_7(M, __VA_ARGS__)
#define HAMU_FOR_EACH_9(M, i, ...) M(i) HAMU_FOR_EACH_8(M, __VA_ARGS__)
#define HAMU_FOR_EACH_10(M, i, ...) M(i) HAMU_FOR_EACH_9(M, __VA_ARGS__)
#define HAMU_FOR_EACH_11(M, i, ...) M(i) HAMU_FOR_EACH_10(M, __VA_ARGS__)
#define HAMU_FOR_EACH_12(M, i, ...) M(i) HAMU_FOR_EACH_11(M, __VA_ARGS__)
#define HAMU_FOR_EACH_13(M, i, ...) M(i) HAMU_FOR_EACH_12(M, __VA_ARGS__)
#define HAMU_FOR_EACH_14(M, i, ...) M(i) HAMU_FOR_EACH_13(M, __VA_ARGS__)
#define HAMU_FOR_EACH_15(M, i, ...) M(i) HAMU_FOR_EACH_14(M, __VA_ARGS__)
#define HAMU_FOR_EACH_16(M, i, ...) M(i) HAMU_FOR_EACH_15(M, __VA_ARGS__)
#define HAMU_FOR_EACH_17(M, i, ...) M(i) HAMU_FOR_EACH_16(M, __VA_ARGS__)
#define HAMU_FOR_EACH_18(M, i, ...) M(i) HAMU_FOR_EACH_17(M, __VA_ARGS__)
#define HAMU_FOR_EACH_19(M, i, ...) M(i) HAMU_FOR_EACH_18(M, __VA_ARGS__)
#define HAMU_FOR_EACH_20(M, i, ...) M(i) HAMU_FOR_EACH_19(M, __VA_ARGS__)
#define HAMU_FOR_EACH_21(M, i, ...) M(i) HAMU_FOR_EACH_20(M, __VA_ARGS__)
#define HAMU_FOR_EACH_22(M, i, ...) M(i) HAMU_FOR_EACH_21(M, __VA_ARGS__)
#define HAMU_FOR_EACH_23(M, i, ...) M(i) HAMU_FOR_EACH_22(M, __VA_ARGS__)
#define HAMU_FOR_EACH_24(M, i, ...) M(i) HAMU_FOR_EACH_23(M, __VA_ARGS__)
#define HAMU_FOR_EACH_25(M, i, ...) M(i) HAMU_FOR_EACH_24(M, __VA_ARGS__)
#define HAMU_FOR_EACH_26(M, i, ...) M(i) HAMU_FOR_EACH_25(M, __VA_ARGS__)
#define HAMU_FOR_EACH_27(M, i, ...) M(i) HAMU_FOR_EACH_26(M, __VA_ARGS__)
#define HAMU_FOR_EACH_28(M, i, ...) M(i) HAMU_FOR_EACH_27(M, __VA_ARGS__)
#define HAMU_FOR_EACH_29(M, i, ...) M(i) HAMU_FOR_EACH_28(M, __VA_ARGS__)
#define HAMU_FOR_EACH_30(M, i, ...) M(i) HAMU_FOR_EACH_29(M, __VA_ARGS__)
#define HAMU_FOR_EACH_31(M, i, ...) M(i) HAMU_FOR_EACH_30(M, __VA_ARGS__)
#define HAMU_FOR_EACH_32(M, i, ...) M(i) HAMU_FOR_EACH_31(M, __VA_ARGS__)
#define HAMU_FOR_EACH_33(M, i, ...) M(i) HAMU_FOR_EACH_32(M, __VA_ARGS__)
#define HAMU_FOR_EACH_34(M, i, ...) M(i) HAMU_FOR_EACH_33(M, __VA_ARGS__)
#define HAMU_FOR_EACH_35(M, i, ...) M(i) HAMU_FOR_EACH_34(M, __VA_ARGS__)
#define HAMU_FOR_EACH_36(M, i, ...) M(i) HAMU_FOR_EACH_35(M, __VA_ARGS__)
#define HAMU_FOR_EACH_37(M, i, ...) M(i) HAMU_FOR_EACH_36(M, __VA_ARGS__)
#define HAMU_FOR_EACH_38(M, i, ...) M(i) HAMU_FOR_EACH_37(M, __VA_ARGS__)
#define HAMU_FOR_EACH_39(M, i, ...) M(i) HAMU_FOR_EACH_38(M, __VA_ARGS__)
#define HAMU_FOR_EACH_40(M, i, ...) M(i) HAMU_FOR_EACH_39(M, __VA_ARGS__)
#define HAMU_FOR_EACH_41(M, i, ...) M(i) HAMU_FOR_EACH_40(M, __VA_ARGS__)
#define HAMU_FOR_EACH_42(M, i, ...) M(i) HAMU_FOR_EACH_41(M, __VA_ARGS__)
#define HAMU_FOR_EACH_43(M, i, ...) M(i) HAMU_FOR_EACH_42(M, __VA_ARGS__)
#define HAMU_FOR_EACH_44(M, i, ...) M(i) HAMU_FOR_EACH_43(M, __VA_ARGS__)
#define HAMU_FOR_EACH_45(M, i, ...) M(i) HAMU_FOR_EACH_44(M, __VA_ARGS__)
#define HAMU_FOR_EACH_46(M, i, ...) M(i) HAMU_FOR_EACH_45(M, __VA_ARGS__)
#define HAMU_FOR_EACH_47(M, i, ...) M(i) HAMU_FOR_EACH_46(M, __VA_ARGS__)
#define HAMU_FOR_EACH_48(M, i, ...) M(i) HAMU_FOR_EACH_47(M, __VA_ARGS__)
#define HAMU_FOR_EACH_49(M, i, ...) M(i) HAMU_FOR_EACH_48(M, __VA_ARGS__)
#define HAMU_FOR_EACH_50(M, i, ...) M(i) HAMU_FOR_EACH_49(M, __VA_ARGS__)
#define HAMU_FOR_EACH_51(M, i, ...) M(i) HAMU_FOR_EACH_50(M, __VA_ARGS__)
#define HAMU_FOR_EACH_52(M, i, ...) M(i) HAMU_FOR_EACH_51(M, __VA_ARGS__)
#define HAMU_FOR_EACH_53(M, i, ...) M(i) HAMU_FOR_EACH_52(M, __VA_ARGS__)
#define HAMU_FOR_EACH_54(M, i, ...) M(i) HAMU_FOR_EACH_53(M, __VA_ARGS__)
#define HAMU_FOR_EACH_55(M, i, ...) M(i) HAMU_FOR_EACH_54(M, __VA_ARGS__)
#define HAMU_FOR_EACH_56(M, i, ...) M(i) HAMU_FOR_EACH_55(M, __VA_ARGS__)
#define HAMU_FOR_EACH_57(M, i, ...) M(i) HAMU_FOR_EACH_56(M, __VA_ARGS__)
#define HAMU_FOR_EACH_58(M, i, ...) M(i) HAMU_FOR_EACH_57(M, __VA_ARGS__)
#define HAMU_FOR_EACH_59(M, i, ...) M(i) HAMU_FOR_EACH_58(M, __VA_ARGS__)
#define HAMU_FOR_EACH_60(M, i, ...) M(i) HAMU_FOR_EACH_59(M, __VA_ARGS__)
#define HAMU_FOR_EACH_61(M, i, ...) M(i) HAMU_FOR_EACH_60(M, __VA_ARGS__)
#define HAMU_FOR_EACH_62(M, i, ...) M(i) HAMU_FOR_EACH_61(M, __VA_ARGS__)
#define HAMU_FOR_EACH_63(M, i, ...) M(i) HAMU_FOR_EACH_62(M, __VA_ARGS__)
#define HAMU_FOR_EACH_64(M, i, ...) M(i) HAMU_FOR_EACH_63(M, __VA_ARGS__)

#define HAMU_APPLY_MACRO_FOR_EACH(M, ...)                                                                              \
    HAMU_CONCAT_EXPAND(HAMU_FOR_EACH_, HAMU_GET_ARGS_COUNT(__VA_ARGS__))                                               \
    (M, __VA_ARGS__)

#define TINY_JSON_DEFINE(Type, ...)                                                                                    \
    namespace hamu::utils                                                                                              \
    {                                                                                                                  \
        void serde(::hamu::utils::JsonWriter& w, const Type& val) noexcept {                                           \
            ::hamu::utils::ObjectSerdeHelper o(w);                                                                     \
            HAMU_APPLY_MACRO_FOR_EACH(HAMU_JSON_FIELD_SERDE, __VA_ARGS__)                                              \
        }                                                                                                              \
        void deserde(::hamu::utils::JsonNode root, Type& object) noexcept {                                            \
            HAMU_APPLY_MACRO_FOR_EACH(HAMU_JSON_FIELD_DESERDE, __VA_ARGS__)                                            \
        }                                                                                                              \
    }

#define INLINE_TINY_JSON_DEFINE(Type, ...)                                                                             \
    namespace hamu::utils                                                                                              \
    {                                                                                                                  \
        inline void serde(::hamu::utils::JsonWriter& w, const Type& val) noexcept {                                    \
            ::hamu::utils::ObjectSerdeHelper o(w);                                                                     \
            HAMU_APPLY_MACRO_FOR_EACH(HAMU_JSON_FIELD_SERDE, __VA_ARGS__)                                              \
        }                                                                                                              \
        inline void deserde(::hamu::utils::JsonNode root, Type& object) noexcept {                                     \
            HAMU_APPLY_MACRO_FOR_EACH(HAMU_JSON_FIELD_DESERDE, __VA_ARGS__)                                            \
        }                                                                                                              \
    }

namespace hamu::utils
{
    inline void serde(JsonWriter& w, std::nullptr_t) noexcept {
        w.writeNull();
    }

    inline void serde(JsonWriter& w, std::monostate) noexcept {
        w.writeNull();
    }

    template <typename T>
    inline void serde(JsonWriter& w, const std::optional<T>& val) noexcept {
        if (val) {
            serde(w, val.value());
            return;
        }
        w.writeNull();
    }

    inline void serde(JsonWriter& w, bool val) noexcept {
        w.writeBool(val);
    }

    inline void serde(JsonWriter& w, const std::string& val) noexcept {
        w.writeString(val);
    }

    inline void serde(JsonWriter& w, std::string_view val) noexcept {
        w.writeString(val);
    }

    inline void serde(JsonWriter& w, const char* val) noexcept {
        w.writeString(val);
    }

    template <JsonNumber T>
    inline void serde(JsonWriter& w, T val) noexcept {
        w.writeNumber(val);
    }

    template <typename T>
    inline void serde(JsonWriter& w, const std::vector<T>& vec) noexcept {
        w.raw('[');
        bool first {true};
        for (const auto& item : vec) {
            if (!first) w.raw(',');
            serde(w, item);
            first = false;
        }
        w.raw(']');
    }

    class ObjectSerdeHelper
    {
    public:
        explicit ObjectSerdeHelper(JsonWriter& w) : w_(w) { w_.raw('{'); }

        ~ObjectSerdeHelper() { w_.raw('}'); }

        template <typename T>
        void field(std::string_view key, const T& val) noexcept {
            if (!first_) w_.raw(',');
            w_.writeKey(key);
            serde(w_, val);
            first_ = false;
        }

    private:
        JsonWriter& w_;
        bool first_ = true;
    };

    /**
     * @brief 将传入对象序列化为 json 字符串
     * 
     * @tparam T 传入对象的 Type
     * @param object 传入的对象
     * @param capacity 为 std::string json 预留的大小, 防止反复扩容, 默认为 256
     * @return std::string 返回的 json 字符串
     */
    template <typename T>
    std::string serde(const T& object, size_t capacity = 256) noexcept {
        std::string json;
        json.reserve(capacity);

        JsonWriter w {json};
        serde(w, object);
        return json;
    }


    // --------------------------------- deserde -----------------------------------

    template <typename T>
    inline void deserde(JsonNode root, std::optional<T>& object) noexcept {
        if (root.isNull()) {
            object = std::nullopt;
        }
        else {
            T val;
            deserde(root, val);
            object = std::move(val);
        }
    }

    inline void deserde(JsonNode root, bool& val) noexcept {
        if (root.isBool()) {
            val = root.get_moved_value<bool>();
            return;
        }
        val = false;
    }

    inline void deserde(JsonNode root, std::string& val) noexcept {
        if (root.isString()) {
            val = root.get_moved_value<std::string>();
            return;
        }
        val = "";
    }

    template <JsonNumber T>
    inline void deserde(JsonNode root, T& val) noexcept {
        if constexpr (std::is_floating_point_v<T>) {
            if (root.isDouble()) {
                val = static_cast<T>(root.get_moved_value<double>());
                return;
            }
        }
        else if constexpr (std::is_integral_v<T>) {
            if (root.isInt64()) {
                val = static_cast<T>(root.get_moved_value<int64_t>());
                return;
            }
        }
        val = 0;
    }

    template <typename T>
    inline void deserde(JsonNode root, std::vector<T>& vec) noexcept {
        const auto& arr = *root.asArrayPtr();
        vec.reserve(arr.size());
        for (const auto& element : arr) {
            vec.emplace_back();
            deserde(element, vec.back());
        }
    }

    /**
     * @brief 使用传入的 json 反序列化为对象
     * 
     * @tparam T 返回对象的 Type
     * @param json json 文本
     * @return T 返回的对象
     */
    template <typename T>
    T deserde(std::string_view json) {
        T object;
        JsonDOMBuilder builder;
        JsonParser parser(builder, json);
        auto result = parser.parser();
        if (result.error()) {
            std::string error_msg {result.info};
            error_msg += std::format(" (line: {}, column: {})", result.line, result.column);
            throw std::runtime_error(error_msg);
        }
        deserde(builder.getRoot(), object);
        return object;
    }

} // namespace hamu::utils