#pragma once

#include "JsonNode.h"

#include <stack>
#include <string_view>

namespace hamu::utils
{

    class JsonDOMBuilder
    {
    public:
        void OnNull();
        void OnBool(bool value);
        void OnNumber(std::string_view number_str);
        void OnString(std::string_view str);
        void OnKey(std::string_view key);

        void OnBeginObject();
        void OnEndObject();
        void OnBeginArray();
        void OnEndArray();

        JsonNode getRoot() noexcept { return std::move(root); }

    private:
        JsonNode root;

        struct Context
        {
            JsonNode* node = nullptr;
            bool isObject  = false;
        };

        std::stack<Context> stack_;
        std::string key_;

        JsonNode* insert(JsonNode&& node);
    };

} // namespace hamu::utils