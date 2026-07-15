#pragma once

#include <cmath>
#include <immintrin.h>
#include "simd/float4_register.hpp"

namespace hamu
{
    struct alignas(16) float4
    {
        float x, y, z, w;

        constexpr float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        constexpr explicit float4(float k) : float4(k, k, k, k) {}
        constexpr float4() : float4(0) {}

        constexpr float* data() { return &x; }
        constexpr const float* data() const { return &x; }
    };

#if defined(__SSE2__)

    inline float4 cvtfloat4_f4reg(const f4reg& reg) {
        float4 out;
        reg.store(out.data());
        return out;
    }

#endif

    inline constexpr float4 operator+(const float4& a, const float4& b) {
        return float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    inline constexpr float4 operator-(const float4& a, const float4& b) {
        return float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    inline constexpr float4 operator*(const float4& a, const float4& b) {
        return float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    inline constexpr float4 operator/(const float4& a, const float4& b) {
        return float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    inline constexpr float4 operator*(const float4& v, float s) {
        return float4(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    inline constexpr float4 operator*(float s, const float4& v) {
        return float4(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    inline constexpr float4 operator/(float s, const float4& v) {
        return float4(s / v.x, s / v.y, s / v.z, s / v.w);
    }

    inline constexpr float4 operator/(const float4& v, float s) {
        return float4(v.x / s, v.y / s, v.z / s, v.w / s);
    }

    inline constexpr float4 operator-(const float4& v) {
        return float4(-v.x, -v.y, -v.z, -v.w);
    }

    inline constexpr float dot(const float4& a, const float4& b) {
#if defined(__SSE2__)
        auto va = f4reg::load_aligned(a.data());
        auto vb = f4reg::load_aligned(b.data());

        auto sum = va * vb;
        sum      = sum + f4reg::merge_high(sum, sum);
        sum      = sum + f4reg::shuffle<1, 0, 0, 0>(sum, sum);

        return f4reg::get_float0(sum);
#else
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
    }

    inline constexpr float length(const float4& v) {
#if defined(__SSE2__)
        auto d = dot(v, v);
        auto x = f4reg::load_constant(d);
        x      = f4reg::sqrt(x);

        return f4reg::get_float0(x);
#else
        return std::sqrt(dot(v, v));
#endif
    }

    inline constexpr float distance(const float4& a, const float4& b) {
        return length(b - a);
    }

    inline constexpr float4 normalize(const float4& v) {
#if defined(__SSE2__)
        auto d     = dot(v, v);
        auto va    = f4reg::load_aligned(v.data());
        auto x     = f4reg::load_constant(d);
        auto half  = f4reg::load_constant(0.5f);
        auto three = f4reg::load_constant(1.5f);

        auto inv  = f4reg::rsqrt(x);
        auto inv2 = inv * inv;
        inv       = inv * f4reg::fast_negmul_add(half * x, inv2, three);
        va        = va * inv;
        return cvtfloat4_f4reg(va);
#else
        return v / length(v);
#endif
    }

    inline constexpr float4 normalize_safe(const float4& v) {
        float len = length(v);
        if (len <= 1e-6) {
            return float4(0.0f);
        }
        return v / len;
    }

    inline constexpr float4 lerp(const float4& a, const float4& b, float t) {
#if defined(__SSE2__)
        auto vt     = f4reg::load_constant(t);
        auto va     = f4reg::load_aligned(a.data());
        auto vb     = f4reg::load_aligned(b.data());
        auto result = f4reg::fast_mul_add(vb - va, vt, va);

        float4 out;
        result.store(out.data());
        return out;
#else
        return a + (b - a) * t;
#endif
    }

    inline constexpr float4 abs(const float4& v) {
#if defined(__SSE2__)
        static const __m128i mask = _mm_set1_epi32(0x7fffffff);

        __m128 va     = _mm_load_ps(v.data());
        __m128 result = _mm_castsi128_ps(_mm_and_si128(_mm_castps_si128(va), mask));

        float4 out;
        _mm_store_ps(out.data(), result);
        return out;
#else
        return float4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
#endif
    }

} // namespace hamu