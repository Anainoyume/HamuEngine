#pragma once

#include <cstdint>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <variant>
#include <vector>

namespace hamu::utils
{
    class JsonNode;
    using JsonObject = std::unordered_map<std::string, JsonNode>;
    using JsonArray  = std::vector<JsonNode>;

    // 没给值, 默认构造第一个类型
    using JsonValue = std::variant<std::monostate, bool, int64_t, double, std::string, JsonObject, JsonArray>;

    enum class JsonType
    {
        Null,
        Bool,
        Int64,
        Double,
        String,
        Object,
        Array
    };

    class JsonNode
    {
    public:
        JsonNode() : value_(std::monostate {}) {}
        JsonNode(bool v) : value_(v) {}
        JsonNode(double v) : value_(v) {}
        JsonNode(int64_t v) : value_(v) {}
        JsonNode(const std::string& v) : value_(v) {}
        JsonNode(std::string&& v) : value_(std::move(v)) {}
        JsonNode(JsonObject&& v) : value_(std::move(v)) {}
        JsonNode(JsonArray&& v) : value_(std::move(v)) {}

        ~JsonNode() = default;

        // clang-format off
        JsonType getType() const {
            if (std::holds_alternative<std::monostate>(value_)) return JsonType::Null;
            if (std::holds_alternative<bool>(value_))           return JsonType::Bool;
            if (std::holds_alternative<double>(value_))         return JsonType::Double;
            if (std::holds_alternative<int64_t>(value_))        return JsonType::Int64;
            if (std::holds_alternative<std::string>(value_))    return JsonType::String;
            if (std::holds_alternative<JsonObject>(value_))     return JsonType::Object;
            if (std::holds_alternative<JsonArray>(value_))      return JsonType::Array;
            return JsonType::Null;
        }

        bool isNull()   const noexcept { return std::holds_alternative<std::monostate>(value_);  }
        bool isBool()   const noexcept { return std::holds_alternative<bool>(value_);            }
        bool isDouble() const noexcept { return std::holds_alternative<double>(value_);          }
        bool isInt64()  const noexcept { return std::holds_alternative<int64_t>(value_);         }
        bool isNumber() const noexcept { return isDouble() || isInt64();                         }
        bool isString() const noexcept { return std::holds_alternative<std::string>(value_);     }
        bool isObject() const noexcept { return std::holds_alternative<JsonObject>(value_);      }
        bool isArray()  const noexcept { return std::holds_alternative<JsonArray>(value_);       }

        const double* asDoublePtr()      const noexcept { return std::get_if<double>(&value_);      }
        const int64_t* asInt64Ptr()      const noexcept { return std::get_if<int64_t>(&value_);     }
        const std::string* asStringPtr() const noexcept { return std::get_if<std::string>(&value_); }
        const bool* asBoolPtr()          const noexcept { return std::get_if<bool>(&value_);        }
        const JsonObject* asObjectPtr()  const noexcept { return std::get_if<JsonObject>(&value_);  }
        const JsonArray* asArrayPtr()    const noexcept { return std::get_if<JsonArray>(&value_);   }
        // clang-format on
        
        template <typename T>
        const T& get() const {
            return std::get<T>(value_);
        }

        template <typename T>
        T get_moved_value() {
            return std::get<T>(std::move(value_));
        }

        void setValue(JsonValue&& v) { value_ = std::move(v); }

        JsonNode& operator[](const std::string& key);
        JsonNode& operator[](size_t index);

        const JsonNode& operator[](const std::string& key) const;
        const JsonNode& operator[](size_t index) const;

        JsonNode* pushToArray(JsonNode&& node);
        JsonNode* pushToObject(std::string&& key, JsonNode&& node);

    private:
        JsonValue value_;

        friend std::ostream& operator<<(std::ostream& os, const JsonNode& node);
        friend void ostreamObject(std::ostream& os, const JsonNode& node);
        friend void ostreamArray(std::ostream& os, const JsonNode& node);
        friend void ostreamValue(std::ostream& os, const JsonNode& node);
    };

} // namespace hamu::utils