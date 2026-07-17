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

    inline simd<8, float>::simd(__m256 m256) noexcept {
        this->_value = m256;
    }

    template <bool Aligned>
    [[nodiscard]]
    inline simd<8, float> simd<8, float>::load(const float* data) noexcept {
        if constexpr (Aligned) {
            return simd<8, float>(_mm256_load_ps(data));
        }
        else {
            return simd<8, float>(_mm256_loadu_ps(data));
        }
    }

    template <bool Aligned>
    inline void simd<8, float>::store(float* data) noexcept {
        if constexpr (Aligned) {
            _mm256_store_ps(data, this->_value);
        }
        else {
            _mm256_storeu_ps(data, this->_value);
        }
    }

    [[nodiscard]]
    inline simd<8, float> simd<8, float>::boardcast(float value) noexcept {
        return simd<8, float>(_mm256_set1_ps(value));
    }

    [[nodiscard]]
    inline simd<8, float> operator+(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_add_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<8, float> operator-(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_sub_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<8, float> operator*(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_mul_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<8, float> operator/(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_div_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<8, float> operator*(simd<8, float> a, float s) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return simd<8, float>(_mm256_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline simd<8, float> operator/(simd<8, float> a, float s) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return simd<8, float>(_mm256_div_ps(a._value, vs));
    }

    [[nodiscard]]
    inline simd<8, float> operator*(float s, simd<8, float> a) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return simd<8, float>(_mm256_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline simd<8, float> operator/(float s, simd<8, float> a) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return simd<8, float>(_mm256_div_ps(a._value, vs));
    }

} // namespace hamu::simd