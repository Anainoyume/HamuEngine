#include "JsonNode.h"
#include "JsonUtils.h"

#include <cstddef>
#include <format>
#include <stdexcept>

namespace hamu::utils
{

    JsonNode& JsonNode::operator[](const std::string& key) {
        if (!isObject()) {
            throw std::runtime_error("JsonNode: Not an object.");
        }
        auto& obj = std::get<JsonObject>(value_);
        return obj[key];
    }

    JsonNode& JsonNode::operator[](size_t index) {
        if (!isArray()) {
            throw std::runtime_error("JsonNode: Not an array.");
        }
        auto& arr = std::get<JsonArray>(value_);
        if (index < arr.size()) return arr[index];
        throw std::out_of_range("Index out of range or not an array");
    }

    const JsonNode& JsonNode::operator[](const std::string& key) const {
        if (!isObject()) {
            throw std::runtime_error("JsonNode: Not an object.");
        }

        const auto& obj = std::get<JsonObject>(value_);
        auto it         = obj.find(key);

        if (it == obj.end()) {
            throw std::runtime_error("JsonNode: Not find key in object.");
        }
        return it->second;
    }

    const JsonNode& JsonNode::operator[](size_t index) const {
        if (!isArray()) {
            throw std::runtime_error("JsonNode: Not an array.");
        }
        const auto& arr = std::get<JsonArray>(value_);
        if (index < arr.size()) return arr[index];
        throw std::out_of_range("Index out of range or not an array");
    }

    JsonNode* JsonNode::pushToArray(JsonNode&& node) {
        if (!isArray()) throw std::logic_error("JsonNode: Not an array.");
        auto& array = std::get<JsonArray>(value_);
        array.push_back(std::move(node));
        return &array.back();
    }

    JsonNode* JsonNode::pushToObject(std::string&& key, JsonNode&& node) {
        if (!isObject()) throw std::logic_error("JsonNode: Not an object.");
        auto [it, insert] = std::get<JsonObject>(value_).insert_or_assign(std::move(key), std::move(node));
        return &(it->second);
    }


    void ostreamObject(std::ostream& os, const JsonNode& node) {
        os << '{';
        const auto& object = *node.asObjectPtr();
        size_t i {};
        for (const auto& [key, node] : object) {
            os << '"' << quotedStringToJson(key) << "\": ";
            // clang-format off
            if      (node.isObject()) { ostreamObject(os, node); }
            else if (node.isArray())  { ostreamArray(os, node);  }
            else                      { ostreamValue(os, node);  }
            // clang-format on
            if (++i < object.size()) os << ", ";
        }
        os << '}';
    }

    void ostreamArray(std::ostream& os, const JsonNode& node) {
        os << '[';
        const auto& array = *node.asArrayPtr();
        size_t i {};
        for (const auto& node : array) {
            // clang-format off
            if      (node.isObject()) { ostreamObject(os, node); }
            else if (node.isArray())  { ostreamArray(os, node);  }
            else                      { ostreamValue(os, node);  }
            // clang-format on
            if (++i < array.size()) os << ", ";
        }
        os << ']';
    }

    void ostreamValue(std::ostream& os, const JsonNode& node) {
        // clang-format off
        switch (node.getType()) {
            case JsonType::Null:   os << "null";                                                break;
            case JsonType::Bool:   os << *node.asBoolPtr();                                     break;
            case JsonType::Int64:  os << *node.asInt64Ptr();                                    break;
            case JsonType::Double: os << *node.asDoublePtr();                                   break;
            case JsonType::String: os << '"' << quotedStringToJson(*node.asStringPtr()) << '"'; break;
            default: break;
        }
        // clang-format on
    }

    std::ostream& operator<<(std::ostream& os, const JsonNode& node) {
        // clang-format off
        if      (node.isObject()) { ostreamObject(os, node); }
        else if (node.isArray())  { ostreamArray(os, node);  }
        else                      { ostreamValue(os, node);  }
        return os;
        // clang-format on
    }



} // namespace hamu::utils