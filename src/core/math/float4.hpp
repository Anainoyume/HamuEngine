#pragma once

#include <cmath>
#include <immintrin.h>
#include <xmmintrin.h>

namespace hamu
{

    struct alignas(16) Float4
    {
        float x, y, z, w;

        constexpr Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        constexpr explicit Float4(float k) : Float4(k, k, k, k) {}
        constexpr Float4() : Float4(0) {}

        constexpr float* data() { return &x; }
        constexpr const float* data() const { return &x; }
    };

    inline constexpr Float4 operator+(const Float4& a, const Float4& b) {
        return Float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline constexpr Float4 operator-(const Float4& a, const Float4& b) {
        return Float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline constexpr Float4 operator*(const Float4& a, const Float4& b) {
        return Float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline constexpr Float4 operator/(const Float4& a, const Float4& b) {
        return Float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline constexpr Float4 operator*(const Float4& v, float s) {
        return Float4(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    inline constexpr Float4 operator*(float s, const Float4& v) {
        return Float4(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    inline constexpr Float4 operator/(float s, const Float4& v) {
        return Float4(s / v.x, s / v.y, s / v.z, s / v.w);
    }

    inline constexpr Float4 operator/(const Float4& v, float s) {
        return Float4(v.x / s, v.y / s, v.z / s, v.w / s);
    }

    inline constexpr Float4 operator-(const Float4& v) {
        return Float4(-v.x, -v.y, -v.z, -v.w);
    }

    inline constexpr float dot(const Float4& a, const Float4& b) {
#if defined(__SSE2__)
        __m128 va = _mm_loadu_ps(a.data());
        __m128 vb = _mm_loadu_ps(b.data());

        __m128 mul = _mm_mul_ps(va, vb);                                // [m0 m1 m2 m3]
        __m128 hi  = _mm_movehl_ps(mul, mul);                           // [m2 m3 m2 m3] 把高两位挪到低两位
        __m128 sum = _mm_add_ps(mul, hi);                               // [m0+m2, m1+m3, ...]
        hi         = _mm_shuffle_ps(sum, sum, _MM_SHUFFLE(0, 0, 0, 1)); // [m1+m3, m0+m2, m0+m2, m0+m2]
        sum        = _mm_add_ss(sum, hi);                               // [dot, ...]
        return _mm_cvtss_f32(sum);
#else
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
    }

    inline constexpr float length(const Float4& v) {
        return std::sqrt(dot(v, v));
    }

    inline constexpr float distance(const Float4& a, const Float4& b) {
        return length(b - a);
    }

    inline constexpr Float4 normalize(const Float4& v) {
#if defined(__SSE2__)
        auto d = dot(v, v);
        __m128 va = _mm_load_ps(v.data());
        __m128 vb = _mm_set_ps1(d);
        vb = _mm_rsqrt_ps(vb);
        va = _mm_mul_ps(va, vb);

        Float4 out;
        _mm_store_ps(out.data(), va);
        return out;
#else
        return v / length(v);
#endif
    }

    inline constexpr Float4 normalize_safe(const Float4& v) {
        float len = length(v);
        if (len <= 1e-6) {
            return Float4(0.0f);
        }
        return v / len;
    }

    inline constexpr Float4 lerp(const Float4& a, const Float4& b, float t) {
        return a + (b - a) * t;
    }

    inline constexpr Float4 abs(const Float4& v) {
        return Float4(std::fabs(v.x), std::fabs(v.y), std::fabs(v.z), std::fabs(v.w));
    }

} // namespace hamu