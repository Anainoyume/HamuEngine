#pragma once

#include "../Simd.hpp"

namespace hamu::simd
{

    template <>
    struct Simd<4, float>
    {
        __m128 _value;

        explicit Simd(__m128 m128) noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        static Simd<4, float> load(const float* data) noexcept;

        [[nodiscard]]
        static Simd<4, float> boardcast(float value) noexcept;

        template <bool Aligned = false>
        void store(float* data) noexcept;
    };

    inline Simd<4, float>::Simd(__m128 m128) noexcept {
        this->_value = m128;
    }

    template <bool Aligned>
    [[nodiscard]]
    inline Simd<4, float> Simd<4, float>::load(const float* data) noexcept {
        if constexpr (Aligned) {
            return Simd<4, float>(_mm_load_ps(data));
        }
        else {
            return Simd<4, float>(_mm_loadu_ps(data));
        }
    }

    template <bool Aligned>
    inline void Simd<4, float>::store(float* data) noexcept {
        if constexpr (Aligned) {
            _mm_store_ps(data, this->_value);
        }
        else {
            _mm_storeu_ps(data, this->_value);
        }
    }

    [[nodiscard]]
    inline Simd<4, float> Simd<4, float>::boardcast(float value) noexcept {
        return Simd<4, float>(_mm_set1_ps(value));
    }

    [[nodiscard]]
    inline Simd<4, float> operator+(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_add_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<4, float> operator-(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_sub_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<4, float> operator*(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_mul_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<4, float> operator/(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_div_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<4, float> operator*(Simd<4, float> a, float s) noexcept {
        const auto vs = _mm_set1_ps(s);
        return Simd<4, float>(_mm_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<4, float> operator/(Simd<4, float> a, float s) noexcept {
        const auto vs = _mm_set1_ps(s);
        return Simd<4, float>(_mm_div_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<4, float> operator*(float s, Simd<4, float> a) noexcept {
        const auto vs = _mm_set1_ps(s);
        return Simd<4, float>(_mm_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<4, float> operator/(float s, Simd<4, float> a) noexcept {
        const auto vs = _mm_set1_ps(s);
        return Simd<4, float>(_mm_div_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<4, float> bit_and(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_and_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<4, float> bit_andnot(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_andnot_ps(a._value, b._value));
    }

    template <size_t Index>
    [[nodiscard]]
    inline float lane(Simd<4, float> v) noexcept {
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
    inline Simd<4, float> merge_high(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_movehl_ps(a._value, b._value));
    }

    template <int X, int Y, int Z, int W>
        requires SimdLane<X> && SimdLane<Y> && SimdLane<Z> && SimdLane<W>
    [[nodiscard]]
    inline Simd<4, float> shuffle(Simd<4, float> a, Simd<4, float> b) noexcept {
        return Simd<4, float>(_mm_shuffle_ps(a._value, b._value, _MM_SHUFFLE(W, Z, Y, X)));
    }

    [[nodiscard]]
    inline Simd<4, float> sqrt(Simd<4, float> v) noexcept {
        return Simd<4, float>(_mm_sqrt_ps(v._value));
    }

    [[nodiscard]]
    inline Simd<4, float> rsqrt(Simd<4, float> v) noexcept {
        return Simd<4, float>(_mm_rsqrt_ps(v._value));
    }

    // Returns a * b + c
    [[nodiscard]]
    inline Simd<4, float> mul_add(Simd<4, float> a, Simd<4, float> b, Simd<4, float> c) noexcept {
        return Simd<4, float>(_mm_fmadd_ps(a._value, b._value, c._value));
    }

    // Returns a - b * c
    [[nodiscard]]
    inline Simd<4, float> sub_mul(Simd<4, float> a, Simd<4, float> b, Simd<4, float> c) noexcept {
        return Simd<4, float>(_mm_fnmadd_ps(b._value, c._value, a._value));
    }

} // namespace hamu::Simd