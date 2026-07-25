#pragma once

#include <concepts>
#include <cstddef>
#include <type_traits>

namespace hamu::simd
{

    template <typename T, size_t Cnt, size_t Pck>
    struct simd_layout
    {
        using data_type                     = T;
        using layout_type                   = simd_layout<T, Cnt, Pck>;
        static constexpr size_t data_count  = Cnt;
        static constexpr size_t data_packet = Pck;

        static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>, "T must be float or double.");
    };

    template <typename T>
    concept simd_type = requires(T t) {
        typename T::layout_type;
        typename T::data_type;

        requires std::derived_from<T, typename T::layout_type>;

        { t.data() } noexcept -> std::same_as<typename T::data_type*>;
    };

} // namespace hamu::simd