#pragma once

#include <immintrin.h>

#if defined(__SSE2__)

namespace hamu
{
    class f4reg
    {
    public:
        f4reg(float w) : _value(_mm_set1_ps(w)) {}

        static f4reg load(const float* data);

        static f4reg load_aligned(const float* data);

        static f4reg load_constant(float w);

        static float get_float0(const f4reg& a);

        // [a.high a.high b.high b.high]
        static f4reg merge_high(const f4reg& a, const f4reg& b);

        // bitmask: 0b 00(从b中选x3) 00(从b中选x2) 00(从a中选x1) 00(从a中选x0), 返回 [x0, x1, x2, x3]
        template <unsigned char bitmask>
        static f4reg shuffle(const f4reg& a, const f4reg& b);

        static f4reg rsqrt(const f4reg& a);

        static f4reg fast_mul_add(
            const f4reg& a, const f4reg& b, const f4reg& c
        );

        static f4reg fast_negmul_add(
            const f4reg& a, const f4reg& b, const f4reg& c
        );

        f4reg operator+(const f4reg& other);

        f4reg operator-(const f4reg& other);

        f4reg operator*(const f4reg& other);

        f4reg operator/(const f4reg& other);

        f4reg operator+(float w);

        f4reg operator-(float w);

        f4reg operator*(float w);

        f4reg operator/(float w);

        f4reg operator&(int bitmask);

        f4reg operator|(int bitmask);

        friend f4reg operator+(float w, const f4reg& other);

        friend f4reg operator-(float w, const f4reg& other);

        friend f4reg operator*(float w, const f4reg& other);

        friend f4reg operator/(float w, const f4reg& other);

        void store(float* data) const;

    private:
        explicit f4reg(__m128 value) : _value(value) {}
        __m128 _value;
    };

    inline f4reg f4reg::load(const float* data) {
        return f4reg(_mm_loadu_ps(data));
    }

    inline f4reg f4reg::load_aligned(const float* data) {
        return f4reg(_mm_load_ps(data));
    }

    inline f4reg f4reg::load_constant(float w) {
        return f4reg(_mm_set1_ps(w));
    }

    inline f4reg f4reg::operator+(const f4reg& other) {
        return f4reg(_mm_add_ps(this->_value, other._value));
    }

    inline f4reg f4reg::operator-(const f4reg& other) {
        return f4reg(_mm_sub_ps(this->_value, other._value));
    }

    inline f4reg f4reg::operator*(const f4reg& other) {
        return f4reg(_mm_mul_ps(this->_value, other._value));
    }

    inline f4reg f4reg::operator/(const f4reg& other) {
        return f4reg(_mm_div_ps(this->_value, other._value));
    }

    inline f4reg f4reg::operator+(float w) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_add_ps(this->_value, t));
    }

    inline f4reg f4reg::operator-(float w) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_sub_ps(this->_value, t));
    }

    inline f4reg f4reg::operator*(float w) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_mul_ps(this->_value, t));
    }

    inline f4reg f4reg::operator/(float w) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_div_ps(this->_value, t));
    }

    inline f4reg f4reg::operator&(int bitmask) {
        const auto mask = _mm_set1_epi32(bitmask);
        return f4reg(_mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(this->_value), mask)));
    }

    inline f4reg f4reg::operator|(int bitmask) {
        const auto mask = _mm_set1_epi32(bitmask);
        return f4reg(_mm_castsi128_ps(_mm_or_si128(_mm_castps_si128(this->_value), mask)));
    }

    inline f4reg operator+(float w, const f4reg& other) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_add_ps(t, other._value));
    }

    inline f4reg operator-(float w, const f4reg& other) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_sub_ps(t, other._value));
    }

    inline f4reg operator*(float w, const f4reg& other) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_mul_ps(t, other._value));
    }

    inline f4reg operator/(float w, const f4reg& other) {
        auto t = _mm_set1_ps(w);
        return f4reg(_mm_div_ps(t, other._value));
    }

    inline void f4reg::store(float* data) const {
        _mm_store_ps(data, this->_value);
    }

    inline float f4reg::get_float0(const f4reg& a) {
        return _mm_cvtss_f32(a._value);
    }

    inline f4reg f4reg::merge_high(const f4reg& a, const f4reg& b) {
        return f4reg(_mm_movehl_ps(a._value, b._value));
    }

    template <unsigned char bitmask>
    inline f4reg f4reg::shuffle(const f4reg& a, const f4reg& b) {
        static_assert(bitmask < 256, "bitmask 必须小于 256");
        return f4reg(_mm_shuffle_ps(a._value, b._value, bitmask));
    }

    inline f4reg f4reg::rsqrt(const f4reg& a) {
        return f4reg(_mm_rsqrt_ps(a._value));
    }

    inline f4reg f4reg::fast_mul_add(
        const f4reg& a, const f4reg& b, const f4reg& c
    ) {
        return f4reg(_mm_fmadd_ps(a._value, b._value, c._value));
    }

    inline f4reg f4reg::fast_negmul_add(
        const f4reg& a, const f4reg& b, const f4reg& c
    ) {
        return f4reg(_mm_fnmadd_ps(a._value, b._value, c._value));
    }

} // namespace hamu

#endif