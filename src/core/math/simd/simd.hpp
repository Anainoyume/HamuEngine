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

    template <int N>
    concept simd_lane = (0 <= N && N < 4);



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

    [[nodiscard]]
    inline simd<4, float> bit_and(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_and_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<4, float> bit_andnot(simd<4, float> a, simd<4, float> b) noexcept {
        return simd<4, float>(_mm_andnot_ps(a._value, b._value));
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

    [[nodiscard]]
    inline simd<8, float> bit_and(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_and_ps(a._value, b._value));
    }

    [[nodiscard]]
    inline simd<8, float> bit_andnot(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_andnot_ps(a._value, b._value));
    }

    template <size_t Index>
    [[nodiscard]]
    inline float lane(simd<8, float> v) noexcept {
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
        requires simd_lane<X> && simd_lane<Y> && simd_lane<Z> && simd_lane<W>
    [[nodiscard]]
    inline simd<8, float> shuffle(simd<8, float> a, simd<8, float> b) noexcept {
        return simd<8, float>(_mm256_shuffle_ps(a._value, b._value, _MM_SHUFFLE(W, Z, Y, X)));
    }

    [[nodiscard]]
    inline simd<8, float> sqrt(simd<8, float> v) noexcept {
        return simd<8, float>(_mm256_sqrt_ps(v._value));
    }

    [[nodiscard]]
    inline simd<8, float> rsqrt(simd<8, float> v) noexcept {
        return simd<8, float>(_mm256_rsqrt_ps(v._value));
    }

    // Returns a * b + c
    [[nodiscard]]
    inline simd<8, float> mul_add(simd<8, float> a, simd<8, float> b, simd<8, float> c) noexcept {
        return simd<8, float>(_mm256_fmadd_ps(a._value, b._value, c._value));
    }

    // Returns a - b * c
    [[nodiscard]]
    inline simd<8, float> sub_mul(simd<8, float> a, simd<8, float> b, simd<8, float> c) noexcept {
        return simd<8, float>(_mm256_fnmadd_ps(b._value, c._value, a._value));
    }

    [[nodiscard]]
    inline simd<8, float> combine(float lo_val, float hi_val) noexcept {
        auto lo = _mm256_castps128_ps256(_mm_set1_ps(lo_val));
        auto hi = _mm_set1_ps(hi_val);
        return simd<8, float>(_mm256_insertf128_ps(lo, hi, 1));
    }

} // namespace hamu::simd