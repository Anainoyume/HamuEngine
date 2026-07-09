#pragma once

#include <concepts>
namespace hamu
{
    template <typename T>
    concept ISerializeReceiveHandler = requires (T& object) {
        { object.OnBeforeSerde() } -> std::same_as<void>;
        { object.OnAfterDeserde() } -> std::same_as<void>;
    };

} // namespace hamu