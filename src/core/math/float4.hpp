#pragma once

#include <cmath>

namespace hamu
{

    struct alignas(16) Float4
    {
        float x, y, z, w;

        constexpr Float4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
        constexpr explicit Float4(float k) : Float4(k, k, k, k) {}
        constexpr Float4() : Float4(0) {}

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
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    inline constexpr float length(const Float4& v) {
        return std::sqrt(dot(v, v));
    }

    inline constexpr float distance(const Float4& a, const Float4& b) {
        return length(b - a);
    }

    inline constexpr Float4 normalize(const Float4& v) {
        return v / length(v);
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