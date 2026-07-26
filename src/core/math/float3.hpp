#pragma once

#include <cmath>

namespace hamu
{

    struct float3
    {
        float x, y, z;

        float3(float x, float y, float z) noexcept : x(x), y(y), z(z) {}

        explicit float3(float k) noexcept : float3(k, k, k) {}

        float3() noexcept : float3(0) {}
    };

    [[nodiscard]]
    inline constexpr float3 operator+(const float3& a, const float3& b) noexcept {
        return float3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    [[nodiscard]]
    inline constexpr float3 operator-(const float3& a, const float3& b) noexcept {
        return float3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    [[nodiscard]]
    inline constexpr float3 operator*(const float3& a, const float3& b) noexcept {
        return float3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    [[nodiscard]]
    inline constexpr float3 operator/(const float3& a, const float3& b) noexcept {
        return float3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    [[nodiscard]]
    inline constexpr float3 operator*(const float3& v, float s) noexcept {
        return float3(v.x * s, v.y * s, v.z * s);
    }

    [[nodiscard]]
    inline constexpr float3 operator*(float s, const float3& v) noexcept {
        return float3(v.x * s, v.y * s, v.z * s);
    }

    [[nodiscard]]
    inline constexpr float3 operator/(float s, const float3& v) noexcept {
        return float3(s / v.x, s / v.y, s / v.z);
    }

    [[nodiscard]]
    inline constexpr float3 operator/(const float3& v, float s) noexcept {
        return float3(v.x / s, v.y / s, v.z / s);
    }
    
    [[nodiscard]]
    inline constexpr float3 operator-(const float3& v) noexcept {
        return float3(-v.x, -v.y, -v.z);
    }

} // namespace hamu