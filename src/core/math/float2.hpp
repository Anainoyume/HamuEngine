#pragma once

#include <cmath>

namespace hamu
{

    struct float2
    {
        float x, y;

        constexpr float2(float x, float y) : x(x), y(y) {}
        constexpr explicit float2(float k) : float2(k, k) {}
        constexpr float2() : float2(0) {}

        constexpr float* data() { return &x; }
        constexpr const float* data() const { return &x; }
    };

    inline constexpr float2 operator+(const float2& a, const float2& b) {
        return float2(a.x + b.x, a.y + b.y);
    }

    inline constexpr float2 operator-(const float2& a, const float2& b) {
        return float2(a.x - b.x, a.y - b.y);
    }

    inline constexpr float2 operator*(const float2& a, const float2& b) {
        return float2(a.x * b.x, a.y * b.y);
    }

    inline constexpr float2 operator/(const float2& a, const float2& b) {
        return float2(a.x / b.x, a.y / b.y);
    }

    inline constexpr float2 operator*(const float2& v, float s) {
        return float2(v.x * s, v.y * s);
    }

    inline constexpr float2 operator*(float s, const float2& v) {
        return float2(v.x * s, v.y * s);
    }

    inline constexpr float2 operator/(float s, const float2& v) {
        return float2(s / v.x, s / v.y);
    }

    inline constexpr float2 operator/(const float2& v, float s) {
        return float2(v.x / s, v.y / s);
    }

    inline constexpr float2 operator-(const float2& v) {
        return float2(-v.x, -v.y);
    }

    inline constexpr float dot(const float2& a, const float2& b) {
        return a.x * b.x + a.y * b.y;
    }

    inline constexpr float length(const float2& v) {
        return std::sqrt(dot(v, v));
    }

    inline constexpr float distance(const float2& a, const float2& b) {
        return length(b - a);
    }

    inline constexpr float2 normalize(const float2& v) {
        return v / length(v);
    }

    inline constexpr float2 normalize_safe(const float2& v) {
        float len = length(v);
        if (len <= 1e-6) {
            return float2(0.0f);
        }
        return v / len;
    }

    inline constexpr float2 lerp(const float2& a, const float2& b, float t) {
        return a + (b - a) * t;
    }

    inline constexpr float2 abs(const float2& v) {
        return float2(std::fabs(v.x), std::fabs(v.y));
    }

} // namespace hamu