#pragma once

#include <cmath>

namespace hamu
{

    struct Float3
    {
        float x, y, z;

        constexpr Float3(float x, float y, float z) : x(x), y(y), z(z) {}
        constexpr explicit Float3(float k) : Float3(k, k, k) {}
        constexpr Float3() : Float3(0) {}

        constexpr float* data() { return &x; }
        constexpr const float* data() const { return &x; }
    };

    inline constexpr Float3 operator+(const Float3& a, const Float3& b) {
        return Float3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    inline constexpr Float3 operator-(const Float3& a, const Float3& b) {
        return Float3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    inline constexpr Float3 operator*(const Float3& a, const Float3& b) {
        return Float3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline constexpr Float3 operator/(const Float3& a, const Float3& b) {
        return Float3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    inline constexpr Float3 operator*(const Float3& v, float s) {
        return Float3(v.x * s, v.y * s, v.z * s);
    }

    inline constexpr Float3 operator*(float s, const Float3& v) {
        return Float3(v.x * s, v.y * s, v.z * s);
    }

    inline constexpr Float3 operator/(float s, const Float3& v) {
        return Float3(s / v.x, s / v.y, s / v.z);
    }

    inline constexpr Float3 operator/(const Float3& v, float s) {
        return Float3(v.x / s, v.y / s, v.z / s);
    }

    inline constexpr Float3 operator-(const Float3& v) {
        return Float3(-v.x, -v.y, -v.z);
    }

    inline constexpr float dot(const Float3& a, const Float3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline constexpr float length(const Float3& v) {
        return std::sqrt(dot(v, v));
    }

    inline constexpr float distance(const Float3& a, const Float3& b) {
        return length(b - a);
    }

    inline constexpr Float3 normalize(const Float3& v) {
        return v / length(v);
    }

    inline constexpr Float3 normalize_safe(const Float3& v) {
        float len = length(v);
        if (len <= 1e-6) {
            return Float3(0.0f);
        }
        return v / len;
    }

    inline constexpr Float3 lerp(const Float3& a, const Float3& b, float t) {
        return a + (b - a) * t;
    }

    inline constexpr Float3 abs(const Float3& v) {
        return Float3(std::fabs(v.x), std::fabs(v.y), std::fabs(v.z));
    }

    inline constexpr Float3 cross(const Float3& a, const Float3& b) {
        return Float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

} // namespace hamu