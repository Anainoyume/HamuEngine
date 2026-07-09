#include "JsonDOMBuilder.h"
#include "JsonNode.h"
#include "JsonUtils.h"

#include <charconv>
#include <memory>
#include <string>
#include <variant>

namespace hamu::utils
{

    void JsonDOMBuilder::OnNull() {
        insert(JsonNode {});
    }

    void JsonDOMBuilder::OnBool(bool value) {
        insert(JsonNode {value});
    }

    void JsonDOMBuilder::OnNumber(std::string_view number_str) {
        bool isDouble {false};
        for (char c : number_str) {
            if (c == '.' || c == 'e' || c == 'E') {
                isDouble = true;
                break;
            }
        }

        if (isDouble) {
            double number {};

            auto [ptr, ec] = std::from_chars(number_str.data(), number_str.data() + number_str.size(), number);
            if (ec != std::errc {}) {
                number = 0.0;
            }

            insert(JsonNode {number});
            return;
        }

        int64_t number {};

        auto [ptr, ec] = std::from_chars(number_str.data(), number_str.data() + number_str.size(), number);
        if (ec == std::errc::result_out_of_range) {
            double double_number {};
            std::from_chars(number_str.data(), number_str.data() + number_str.size(), double_number);
            if (ec == std::errc {}) {
                insert(JsonNode {double_number});
                return;
            }
        }

        if (ec != std::errc {}) {
            number = 0;
        }

        insert(JsonNode {number});
    }

    void JsonDOMBuilder::OnString(std::string_view str) {
        insert(JsonNode {flattenedString(str)});
    }

    void JsonDOMBuilder::OnKey(std::string_view key) {
        key_ = flattenedString(key);
    }

    void JsonDOMBuilder::OnBeginObject() {
        JsonNode object(JsonObject {});
        auto p = insert(std::move(object));
        stack_.push(Context {p, true});
    }

    void JsonDOMBuilder::OnEndObject() {
        stack_.pop();
    }

    void JsonDOMBuilder::OnBeginArray() {
        JsonNode arr(JsonArray {});
        auto p = insert(std::move(arr));
        stack_.push({p, false});
    }

    void JsonDOMBuilder::OnEndArray() {
        stack_.pop();
    }

    JsonNode* JsonDOMBuilder::insert(JsonNode&& node) {
        if (stack_.empty()) {
            root = std::move(node);
            return &root;
        }

        const auto& ctx = stack_.top();
        if (ctx.isObject) return ctx.node->pushToObject(std::move(key_), std::move(node));
        return ctx.node->pushToArray(std::move(node));
    }

} // namespace hamu::utils