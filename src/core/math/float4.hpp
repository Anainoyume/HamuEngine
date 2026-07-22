#pragma once

#include <cmath>
#include <immintrin.h>

#include "simd/Simd.hpp"
#include "simd/SimdExt.hpp"

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
        using Simd4f = simd::Simd<4, float>;
        auto va      = Simd4f::load<true>(a.data());
        auto vb      = Simd4f::load<true>(b.data());
        return simd::Dot(va, vb);
#else
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
#endif
    }

    inline constexpr float length(const Float4& v) {
#if defined(__SSE2__)
        using Simd4f = simd::Simd<4, float>;
        auto vv      = Simd4f::load<true>(v.data());
        return simd::Length(vv);
#else
        return std::sqrt(dot(v, v));
#endif
    }

    inline constexpr float distance(const Float4& a, const Float4& b) {
        return length(b - a);
    }

    inline constexpr Float4 normalize(const Float4& v) {
#if defined(__SSE2__)
        using Simd4f = simd::Simd<4, float>;
        auto vv      = Simd4f::load<true>(v.data());
        auto x       = simd::Normalize(vv);

        Float4 result;
        x.store(result.data());
        return result;
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
#if defined(__SSE2__)
        using Simd4f = simd::Simd<4, float>;
        auto va      = Simd4f::load<true>(a.data());
        auto vb      = Simd4f::load<true>(b.data());
        auto x       = simd::Lerp(va, vb, t);

        Float4 result;
        x.store<true>(result.data());
        return result;
#else
        return a + (b - a) * t;
#endif
    }

    inline constexpr Float4 abs(const Float4& v) {
#if defined(__SSE2__)
        using Simd4f = simd::Simd<4, float>;
        auto va      = Simd4f::load<true>(v.data());
        auto x       = simd::Abs(va);

        Float4 result;
        x.store<true>(result.data());
        return result;
#else
        return Float4(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
#endif
    }

} // namespace hamu