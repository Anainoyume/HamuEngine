#pragma once

#include <cstddef>
#include <immintrin.h>

namespace hamu::simd
{

    template <size_t N, typename T>
    struct simd;

    template <>
    struct simd<8, float>
    {
        __m256 _value;

        explicit simd(__m256 m128) noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        static simd<8, float> load(const float* data) noexcept;

        [[nodiscard]]
        static simd<8, float> boardcast(float value) noexcept;

        template <bool Aligned = false>
        void store(float* data) noexcept;
    };

} // namespace hamu::simd