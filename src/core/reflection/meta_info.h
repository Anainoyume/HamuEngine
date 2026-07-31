#pragma once

#include <array>
#include <string_view>
#include <vector>

#include "meta_id.h"

namespace hamu
{

    struct meta_info;

    struct meta_field
    {
        meta_info* type;
        std::string_view name;
        size_t offset;
    };

    struct meta_info
    {
        meta_info* parent;
        std::string_view name;
        std::vector<meta_field> fields;

        meta_info() {}
        meta_info(meta_info* parent, std::string_view name) : parent(parent), name(name) {}

        void add_field(meta_info* type, std::string_view name, size_t offset) noexcept {
            fields.emplace_back(type, name, offset);
        }
    };

    inline std::array<meta_info, meta_count> hamu_meta_register;

    [[nodiscard]]
    inline constexpr const meta_info* metaof(meta_id id) noexcept {
        auto idx = static_cast<int>(id);
        if (idx < 0 || idx >= meta_count) return nullptr;
        return &hamu_meta_register[idx];
    }

    struct meta_register
    {
        template <typename Func>
        meta_register(Func&& register_func) noexcept {
            register_func();
        }
    };

    struct Object
    {
        int uuid;
        std::string name;

        static int version;

        Object() {}
        ~Object() {}

        Object(int id) : uuid(id) {}
    };

    inline static meta_register hamu_meta_register_object([]() {
        auto idx                = static_cast<int>(meta_id::object);
        auto& obj = (hamu_meta_register[idx] = meta_info(nullptr, "Object"));
    });

} // namespace hamu