#pragma once

#include <cmath>
#include <immintrin.h>

#include "simd/simd.hpp"
#include "simd/simd_ext.hpp"
#include "simd/simd_layout.hpp"

namespace hamu
{
    struct alignas(16) float4 : public simd::simd_layout<float, 4, 1>
    {
        float x, y, z, w;

        constexpr float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

        constexpr explicit float4(float k) : float4(k, k, k, k) {}

        constexpr float4() : float4(0) {}

        float* data() noexcept { return &x; }
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

} // namespace hamu