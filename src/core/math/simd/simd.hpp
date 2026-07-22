#pragma once

#include <cstddef>
#include <immintrin.h>

namespace hamu::simd
{

    template <typename>
    inline constexpr bool dependent_false = false;

    template <size_t N, typename T>
    struct Simd
    {
        static_assert(dependent_false<T>, "Template parameters are invalid.");
    };

    template <int N>
    concept SimdLane = (0 <= N && N < 4);

} // namespace hamu::simd

#include "detail/Simd4f.inl"
#include "detail/Simd8f.inl"