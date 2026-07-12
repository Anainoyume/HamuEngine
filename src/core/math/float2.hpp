#pragma once

#include <cmath>

namespace hamu
{

    struct Float2
    {
        float x, y;

        constexpr Float2(float x, float y) : x(x), y(y) {}
        constexpr explicit Float2(float k) : Float2(k, k) {}
        constexpr Float2() : Float2(0) {}

        constexpr const float* data() const { return &x; }
    };

    inline constexpr Float2 operator+(const Float2& a, const Float2& b) {
        return Float2(a.x + b.x, a.y + b.y);
    }

    inline constexpr Float2 operator-(const Float2& a, const Float2& b) {
        return Float2(a.x - b.x, a.y - b.y);
    }

    inline constexpr Float2 operator*(const Float2& a, const Float2& b) {
        return Float2(a.x * b.x, a.y * b.y);
    }

    inline constexpr Float2 operator/(const Float2& a, const Float2& b) {
        return Float2(a.x / b.x, a.y / b.y);
    }

    inline constexpr Float2 operator*(const Float2& v, float s) {
        return Float2(v.x * s, v.y * s);
    }

    inline constexpr Float2 operator*(float s, const Float2& v) {
        return Float2(v.x * s, v.y * s);
    }

    inline constexpr Float2 operator/(float s, const Float2& v) {
        return Float2(s / v.x, s / v.y);
    }

    inline constexpr Float2 operator/(const Float2& v, float s) {
        return Float2(v.x / s, v.y / s);
    }

    inline constexpr Float2 operator-(const Float2& v) {
        return Float2(-v.x, -v.y);
    }

    inline constexpr float dot(const Float2& a, const Float2& b) {
        return a.x * b.x + a.y * b.y;
    }

    inline constexpr float length(const Float2& v) {
        return std::sqrt(dot(v, v));
    }

    inline constexpr float distance(const Float2& a, const Float2& b) {
        return length(b - a);
    }

    inline constexpr Float2 normalize(const Float2& v) {
        return v / length(v);
    }

    inline constexpr Float2 normalize_safe(const Float2& v) {
        float len = length(v);
        if (len <= 1e-6) {
            return Float2(0.0f);
        }
        return v / len;
    }

    inline constexpr Float2 lerp(const Float2& a, const Float2& b, float t) {
        return a + (b - a) * t;
    }

    inline constexpr Float2 abs(const Float2& v) {
        return Float2(std::fabs(v.x), std::fabs(v.y));
    }

} // namespace hamu