#pragma once

#include "../Simd.hpp"

namespace hamu::simd
{

    template <>
    struct Simd<8, float>
    {
        __m256 _value;

        explicit Simd(__m256 m128) noexcept;

        template <bool Aligned = false>
        [[nodiscard]]
        static Simd<8, float> load(const float* data) noexcept;

        [[nodiscard]]
        static Simd<8, float> boardcast(float value) noexcept;

        template <bool Aligned = false>
        void store(float* data) noexcept;
    };

    inline Simd<8, float>::Simd(__m256 m256) noexcept {
        this->_value = m256;
    }

    template <bool Aligned>
    [[nodiscard]]
    inline Simd<8, float> Simd<8, float>::load(const float* data) noexcept {
        if constexpr (Aligned) {
            return Simd<8, float>(_mm256_load_ps(data));
        }
        else {
            return Simd<8, float>(_mm256_loadu_ps(data));
        }
    }

    template <bool Aligned>
    inline void Simd<8, float>::store(float* data) noexcept {
        if constexpr (Aligned) {
            _mm256_store_ps(data, this->_value);
        }
        else {
            _mm256_storeu_ps(data, this->_value);
        }
    }

    [[nodiscard]]
    inline Simd<8, float> Simd<8, float>::boardcast(float value) noexcept {
        return Simd<8, float>(_mm256_set1_ps(value));
    }

    [[nodiscard]]
    inline Simd<8, float> operator+(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_add_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<8, float> operator-(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_sub_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<8, float> operator*(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_mul_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<8, float> operator/(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_div_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<8, float> operator*(Simd<8, float> a, float s) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return Simd<8, float>(_mm256_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<8, float> operator/(Simd<8, float> a, float s) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return Simd<8, float>(_mm256_div_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<8, float> operator*(float s, Simd<8, float> a) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return Simd<8, float>(_mm256_mul_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<8, float> operator/(float s, Simd<8, float> a) noexcept {
        const auto vs = _mm256_set1_ps(s);
        return Simd<8, float>(_mm256_div_ps(a._value, vs));
    }

    [[nodiscard]]
    inline Simd<8, float> bit_and(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_and_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline Simd<8, float> bit_andnot(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_andnot_ps(a._value, b._value));
    }

    template <size_t Index>
    [[nodiscard]]
    inline float lane(Simd<8, float> v) noexcept {
        if constexpr (Index == 0) {
            return _mm256_cvtss_f32(v._value);
        }
        else if constexpr (Index < 4) {
            // 低 128-bit lane，用 _mm_shuffle_ps 把目标搬到 lane 0
            auto lo = _mm256_castps256_ps128(v._value);
            auto m  = shuffle<Index, Index, Index, Index>(lo, lo);
            return lane<0>(m);
        }
        else if constexpr (Index == 4) {
            auto hi = _mm256_extractf128_ps(v._value, 1);
            return _mm_cvtss_f32(hi);
        }
        else {
            // 高 128-bit lane，先 extract 再 shuffle
            auto hi           = _mm256_extractf128_ps(v._value, 1);
            constexpr int Sub = Index - 4;
            auto m            = shuffle<Sub, Sub, Sub, Sub>(hi, hi);
            return lane<0>(m);
        }
    }

    template <int X, int Y, int Z, int W>
        requires SimdLane<X> && SimdLane<Y> && SimdLane<Z> && SimdLane<W>
    [[nodiscard]]
    inline Simd<8, float> shuffle(Simd<8, float> a, Simd<8, float> b) noexcept {
        return Simd<8, float>(_mm256_shuffle_ps(a._value, b._value, _MM_SHUFFLE(W, Z, Y, X)));
    }

    [[nodiscard]]
    inline Simd<8, float> sqrt(Simd<8, float> v) noexcept {
        return Simd<8, float>(_mm256_sqrt_ps(v._value));
    }

    [[nodiscard]]
    inline Simd<8, float> rsqrt(Simd<8, float> v) noexcept {
        return Simd<8, float>(_mm256_rsqrt_ps(v._value));
    }

    // Returns a * b + c
    [[nodiscard]]
    inline Simd<8, float> mul_add(Simd<8, float> a, Simd<8, float> b, Simd<8, float> c) noexcept {
        return Simd<8, float>(_mm256_fmadd_ps(a._value, b._value, c._value));
    }

    // Returns a - b * c
    [[nodiscard]]
    inline Simd<8, float> sub_mul(Simd<8, float> a, Simd<8, float> b, Simd<8, float> c) noexcept {
        return Simd<8, float>(_mm256_fnmadd_ps(b._value, c._value, a._value));
    }

    [[nodiscard]]
    inline Simd<8, float> combine(float lo_val, float hi_val) noexcept {
        auto lo = _mm256_castps128_ps256(_mm_set1_ps(lo_val));
        auto hi = _mm_set1_ps(hi_val);
        return Simd<8, float>(_mm256_insertf128_ps(lo, hi, 1));
    }

} // namespace hamu::Simd