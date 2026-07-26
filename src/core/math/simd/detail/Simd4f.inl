#pragma once

#include "../simd.hpp"

namespace hamu::simd
{

    template <>
    struct simd<4, float>
    {
        __m128 _value;

        explicit simd(__m128 m128) noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        static simd<4, float> load(float* data) noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        static simd<4, float> load(const float* data) noexcept;

        [[nodiscard]]
        static simd<4, float> boardcast(float value) noexcept;

        template <bool Aligned = false>
        void store(float* data) noexcept;
    };

    inline const auto SIMD4F_CONST_NEG_ZERO   = simd<4, float>(_mm_set1_ps(-0.0f));
    inline const auto SIMD4F_CONST_THREE_HALF = simd<4, float>(_mm_set1_ps(1.5f));
    inline const auto SIMD4F_CONST_HALF       = simd<4, float>(_mm_set1_ps(0.5f));


    inline simd<4, float>::simd(__m128 m128) noexcept {
        this->_value = m128;
    }

    template <bool Aligned>
    [[nodiscard]]
    inline simd<4, float> simd<4, float>::load(float* data) noexcept {
        if constexpr (Aligned) {
            return simd<4, float>(_mm_load_ps(data));
        }
        else {
            return simd<4, float>(_mm_loadu_ps(data));
        }
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

    [[nodiscard]]
    inline simd<4, float> bit_and(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_and_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> bit_andnot(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_andnot_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> bit_or(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_or_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> bit_xor(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_xor_ps(a._value, b._value));
    }

    template <size_t Index>
    [[nodiscard]]
    inline float lane(simd<4, float> v) noexcept {
        if constexpr (Index == 0) {
            return _mm_cvtss_f32(v._value);
        }
        else {
            auto m = shuffle<Index, Index, Index, Index>(v, v);
            return lane<0>(m);
        }
    }

    // Returns [b.high | a.high]  (lane3 ---> lane0)
    [[nodiscard]]
    inline simd<4, float> merge_high(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_movehl_ps(a._value, b._value));
    }

    template <int X, int Y, int Z, int W>
    [[nodiscard]]
    inline simd<4, float> shuffle(simd<4, float> a, simd<4, float> b) noexcept {
        static_assert(
            (0 <= X && X < 4) && (0 <= Y && Y < 4) && (0 <= Z && Z < 4) && (0 <= W && W < 4),
            "The index must be between 0 and 3 (inclusive)."
        );

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