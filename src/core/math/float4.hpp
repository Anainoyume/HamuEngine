#pragma once

#include <cmath>
#include <immintrin.h>

#include "simd/simd.hpp"
#include "simd/simd_ext.hpp"

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
        return simd::Dot(va, vb);
#else
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
    }

    inline constexpr float length(const float4& v) {
#if defined(__SSE2__)
        using simd4f = simd::simd<4, float>;
        auto vv      = simd4f::load<true>(v.data());
        return simd::Length(vv);
#else
        return std::sqrt(dot(v, v));
#endif
    }

    inline constexpr float distance(const float4& a, const float4& b) {
        return length(b - a);
    }

    inline constexpr float4 normalize(const float4& v) {
#if defined(__SSE2__)
        using simd4f = simd::simd<4, float>;
        auto vv      = simd4f::load<true>(v.data());
        auto x       = simd::Normalize(vv);

        float4 result;
        x.store(result.data());
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
        auto va      = simd4f::load<true>(a.data());
        auto vb      = simd4f::load<true>(b.data());
        auto x       = simd::Lerp(va, vb, t);

        float4 result;
        x.store<true>(result.data());
        return result;
#else
        return a + (b - a) * t;
#endif
    }

    inline constexpr float4 abs(const float4& v) {
#if defined(__SSE2__)
        using simd4f = simd::simd<4, float>;
        auto va      = simd4f::load<true>(v.data());
        auto x       = simd::Abs(va);

        float4 result;
        x.store<true>(result.data());
        return result;
#else
        return float4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
#endif
    }

} // namespace hamu