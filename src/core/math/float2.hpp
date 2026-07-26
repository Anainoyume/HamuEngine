#pragma once

#include <cmath>

namespace hamu
{

    struct float2
    {
        float x, y;

        float2(float x, float y) noexcept : x(x), y(y) {}

        explicit float2(float k) noexcept : float2(k, k) {}

        float2() noexcept : float2(0) {}
    };

    [[nodiscard]]
    inline constexpr float2 operator+(const float2& a, const float2& b) noexcept {
        return float2(a.x + b.x, a.y + b.y);
    }

    [[nodiscard]]
    inline constexpr float2 operator-(const float2& a, const float2& b) noexcept {
        return float2(a.x - b.x, a.y - b.y);
    }

    [[nodiscard]]
    inline constexpr float2 operator*(const float2& a, const float2& b) noexcept {
        return float2(a.x * b.x, a.y * b.y);
    }

    [[nodiscard]]
    inline constexpr float2 operator/(const float2& a, const float2& b) noexcept {
        return float2(a.x / b.x, a.y / b.y);
    }

    [[nodiscard]]
    inline constexpr float2 operator*(const float2& v, float s) noexcept {
        return float2(v.x * s, v.y * s);
    }

    [[nodiscard]]
    inline constexpr float2 operator*(float s, const float2& v) noexcept {
        return float2(v.x * s, v.y * s);
    }

    [[nodiscard]]
    inline constexpr float2 operator/(float s, const float2& v) noexcept {
        return float2(s / v.x, s / v.y);
    }

    [[nodiscard]]
    inline constexpr float2 operator/(const float2& v, float s) noexcept {
        return float2(v.x / s, v.y / s);
    }

    [[nodiscard]]
    inline constexpr float2 operator-(const float2& v) noexcept {
        return float2(-v.x, -v.y);
    }

} // namespace hamu