#pragma once

#include <cstddef>
#include <immintrin.h>

namespace hamu::simd
{

    template <typename>
    inline constexpr bool dependent_false = false;

    template <size_t N, typename T>
    struct simd
    {
        static_assert(dependent_false<T>, "Template parameters are invalid.");
    };

} // namespace hamu::simd

#include "detail/simd4f.inl"
#include "detail/simd8f.inl"