#pragma once

#include <cmath>
#include <immintrin.h>

#include "simd/simd.hpp"
#include "simd/simd_float4.inl"

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
        using simd4f = simd::simd<4, float>;
        auto va      = simd4f::load<true>(a.data());
        auto vb      = simd4f::load<true>(b.data());

        auto sum = va * vb;
        sum      = sum + simd::merge_high(sum, sum);
        sum      = sum + simd::shuffle<1, 0, 0, 0>(sum, sum);

        return simd::lane<0>(sum);
#else
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
    }

    inline constexpr float length(const float4& v) {
#if defined(__SSE2__)
        using simd4f = simd::simd<4, float>;
        auto d       = dot(v, v);
        auto x       = simd4f::boardcast(d);
        x            = simd::sqrt(x);

        return simd::lane<0>(x);
#else
        return std::sqrt(dot(v, v));
#endif
    }

    inline constexpr float distance(const float4& a, const float4& b) {
        return length(b - a);
    }

    inline constexpr float4 normalize(const float4& v) {
#if defined(__SSE2__)
        using simd4f            = simd::simd<4, float>;
        auto d                  = dot(v, v);
        auto va                 = simd4f::load<true>(v.data());
        auto x                  = simd4f::boardcast(d);
        static const auto half  = simd4f::boardcast(0.5f);
        static const auto three = simd4f::boardcast(1.5f);

        auto inv  = simd::rsqrt(x);
        auto inv2 = inv * inv;
        inv       = inv * simd::sub_mul(three, x * half, inv2);
        va        = va * inv;

        float4 result;
        va.store<true>(result.data());
        return result;
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
        using simd4f = simd::simd<4, float>;
        auto vt      = simd4f::boardcast(t);
        auto va      = simd4f::load<true>(a.data());
        auto vb      = simd4f::load<true>(b.data());
        auto x       = simd::mul_add(vb - va, vt, va);

        float4 result;
        x.store<true>(result.data());
        return result;
#else
        return a + (b - a) * t;
#endif
    }

    inline constexpr float4 abs(const float4& v) {
#if defined(__SSE2__)
        using simd4f           = simd::simd<4, float>;
        static const auto mask = simd4f::boardcast(-0.0f);
        auto va                = simd4f::load<true>(v.data());
        auto x                 = simd::bit_andnot(mask, va);

        float4 result;
        x.store<true>(result.data());
        return result;
#else
        return float4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
#endif
    }

} // namespace hamu