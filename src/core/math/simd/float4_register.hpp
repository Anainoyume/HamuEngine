#pragma once

#include <immintrin.h>

#if defined(__SSE2__)

namespace hamu
{
    class f4reg
    {
    public:
        static f4reg load(const float* data);

        static f4reg load_aligned(const float* data);

        static f4reg load_constant(float w);

        static float get_float0(const f4reg& a);

        static f4reg merge_high(const f4reg& a, const f4reg& b);

        template <int X, int Y, int Z, int W>
        static f4reg shuffle(const f4reg& a, const f4reg& b);

        static f4reg rsqrt(const f4reg& a);

        static f4reg fast_mul_add(const f4reg& a, const f4reg& b, const f4reg& c);

        static f4reg fast_negmul_add(const f4reg& a, const f4reg& b, const f4reg& c);

        f4reg operator+(const f4reg& other);

        f4reg operator-(const f4reg& other);

        f4reg operator*(const f4reg& other);

        f4reg operator/(const f4reg& other);

        f4reg& operator+=(const f4reg& other);

        f4reg& operator-=(const f4reg& other);

        f4reg& operator*=(const f4reg& other);

        f4reg& operator/=(const f4reg& other);

        f4reg operator+(float w);

        f4reg operator-(float w);

        f4reg operator*(float w);

        f4reg operator/(float w);

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

    inline f4reg& f4reg::operator+=(const f4reg& other) {
        this->_value = _mm_add_ps(this->_value, other._value);
        return *this;
    }

    inline f4reg& f4reg::operator-=(const f4reg& other) {
        this->_value = _mm_sub_ps(this->_value, other._value);
        return *this;
    }

    inline f4reg& f4reg::operator*=(const f4reg& other) {
        this->_value = _mm_mul_ps(this->_value, other._value);
        return *this;
    }

    inline f4reg& f4reg::operator/=(const f4reg& other) {
        this->_value = _mm_div_ps(this->_value, other._value);
        return *this;
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

    template <int X, int Y, int Z, int W>
    inline f4reg f4reg::shuffle(const f4reg& a, const f4reg& b) {
        static_assert(
            X >= 0 && X < 4 && Y >= 0 && Y < 4 && Z >= 0 && Z < 4 && W >= 0 && W < 4,
            "shuffle index must be in range [0,3]"
        );

        constexpr int mask = X | (Y << 2) | (Z << 4) | (W << 6);

        return f4reg(_mm_shuffle_ps(a._value, b._value, mask));
    }

    inline f4reg f4reg::rsqrt(const f4reg& a) {
        return f4reg(_mm_rsqrt_ps(a._value));
    }

    inline f4reg f4reg::fast_mul_add(const f4reg& a, const f4reg& b, const f4reg& c) {
        return f4reg(_mm_fmadd_ps(a._value, b._value, c._value));
    }

    inline f4reg f4reg::fast_negmul_add(const f4reg& a, const f4reg& b, const f4reg& c) {
        return f4reg(_mm_fnmadd_ps(a._value, b._value, c._value));
    }

} // namespace hamu

#endif