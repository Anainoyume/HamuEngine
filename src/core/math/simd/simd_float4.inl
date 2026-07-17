#pragma once

#include <cstddef>
#include <immintrin.h>
#include <xmmintrin.h>

namespace hamu::simd
{

    template <size_t N, typename T>
    struct simd;

    template <>
    struct simd<4, float>
    {
        __m128 _value;

        explicit simd(__m128 m128) noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        static simd<4, float> load(const float* data) noexcept;

        [[nodiscard]]
        static simd<4, float> boardcast(float value) noexcept;

        template <bool Aligned = false>
        void store(float* data) noexcept;
    };

    inline simd<4, float>::simd(__m128 m128) noexcept {
        this->_value = m128;
    }

    template <bool Aligned>
    [[nodiscard]]
    inline simd<4, float> simd<4, float>::load(const float* data) noexcept {
        if constexpr (Aligned) {
            return simd<4, float>(_mm_load_ps(data));
        }
        else {
            return simd<4, float>(_mm_loadu_ps(data));
        }
    }

    template <bool Aligned>
    inline void simd<4, float>::store(float* data) noexcept {
        if constexpr (Aligned) {
            _mm_store_ps(data, this->_value);
        }
        else {
            _mm_storeu_ps(data, this->_value);
        }
    }

    [[nodiscard]]
    inline simd<4, float> simd<4, float>::boardcast(float value) noexcept {
        return simd<4, float>(_mm_set1_ps(value));
    }

    [[nodiscard]]
    inline simd<4, float> operator+(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_add_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> operator-(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_sub_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> operator*(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_mul_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> operator/(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_div_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> operator*(simd<4, float> a, float s) noexcept {
        const auto vs = _mm_set1_ps(s);
        return simd<4, float>(_mm_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline simd<4, float> operator/(simd<4, float> a, float s) noexcept {
        const auto vs = _mm_set1_ps(s);
        return simd<4, float>(_mm_div_ps(a._value, vs));
    }

    [[nodiscard]]
    inline simd<4, float> operator*(float s, simd<4, float> a) noexcept {
        const auto vs = _mm_set1_ps(s);
        return simd<4, float>(_mm_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline simd<4, float> operator/(float s, simd<4, float> a) noexcept {
        const auto vs = _mm_set1_ps(s);
        return simd<4, float>(_mm_div_ps(a._value, vs));
    }

    template <size_t Index>
    [[nodiscard]]
    float lane(simd<4, float> v) noexcept;

    template <>
    [[nodiscard]]
    float lane<0>(simd<4, float> v) noexcept {
        return _mm_cvtss_f32(v._value);
    }

    // Returns [b.high | a.high]  (lane3 ---> lane0)
    [[nodiscard]]
    inline simd<4, float> merge_high(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_movehl_ps(a._value, b._value));
    }

    template <int N>
    concept simd_lane = (0 <= N && N < 4);

    template <int X, int Y, int Z, int W>
        requires simd_lane<X> && simd_lane<Y> && simd_lane<Z> && simd_lane<W>
    [[nodiscard]]
    inline simd<4, float> shuffle(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_shuffle_ps(a._value, b._value, _MM_SHUFFLE(W, Z, Y, X)));
    }

    [[nodiscard]]
    inline simd<4, float> sqrt(simd<4, float> v) noexcept {
        return simd<4, float>(_mm_sqrt_ps(v._value));
    }

    [[nodiscard]]
    inline simd<4, float> rsqrt(simd<4, float> v) noexcept {
        return simd<4, float>(_mm_rsqrt_ps(v._value));
    }

    // Returns a * b + c
    [[nodiscard]]
    inline simd<4, float> mul_add(simd<4, float> a, simd<4, float> b, simd<4, float> c) noexcept {
        return simd<4, float>(_mm_fmadd_ps(a._value, b._value, c._value));
    }

    // Returns a - b * c
    [[nodiscard]]
    inline simd<4, float> sub_mul(simd<4, float> a, simd<4, float> b, simd<4, float> c) noexcept {
        return simd<4, float>(_mm_fnmadd_ps(b._value, c._value, a._value));
    }

} // namespace hamu::simd