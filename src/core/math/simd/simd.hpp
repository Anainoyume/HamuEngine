#pragma once

#include <cstddef>
#include <immintrin.h>

namespace hamu::simd
{

    template <size_t N, typename T>
    struct simd
    {
        static_assert(false, "Template parameters are invalid.");
    };

} // namespace hamu::simd

#include "detail/simd4f.inl"
#include "detail/simd8f.inl"