#pragma once

#include <cmath>

namespace hamu
{

    struct float3
    {
        float x, y, z;

        constexpr float3(float x, float y, float z) : x(x), y(y), z(z) {}
        constexpr explicit float3(float k) : float3(k, k, k) {}
        constexpr float3() : float3(0) {}

        constexpr float* data() { return &x; }
        constexpr const float* data() const { return &x; }
    };

    inline constexpr float3 operator+(const float3& a, const float3& b) {
        return float3(a.x + b.x, a.y + b.y, a.z + b.z);
    }

    inline constexpr float3 operator-(const float3& a, const float3& b) {
        return float3(a.x - b.x, a.y - b.y, a.z - b.z);
    }

    inline constexpr float3 operator*(const float3& a, const float3& b) {
        return float3(a.x * b.x, a.y * b.y, a.z * b.z);
    }

    inline constexpr float3 operator/(const float3& a, const float3& b) {
        return float3(a.x / b.x, a.y / b.y, a.z / b.z);
    }

    inline constexpr float3 operator*(const float3& v, float s) {
        return float3(v.x * s, v.y * s, v.z * s);
    }

    inline constexpr float3 operator*(float s, const float3& v) {
        return float3(v.x * s, v.y * s, v.z * s);
    }

    inline constexpr float3 operator/(float s, const float3& v) {
        return float3(s / v.x, s / v.y, s / v.z);
    }

    inline constexpr float3 operator/(const float3& v, float s) {
        return float3(v.x / s, v.y / s, v.z / s);
    }

    inline constexpr float3 operator-(const float3& v) {
        return float3(-v.x, -v.y, -v.z);
    }

    inline constexpr float dot(const float3& a, const float3& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    inline constexpr float length(const float3& v) {
        return std::sqrt(dot(v, v));
    }

    inline constexpr float distance(const float3& a, const float3& b) {
        return length(b - a);
    }

    inline constexpr float3 normalize(const float3& v) {
        return v / length(v);
    }

    inline constexpr float3 normalize_safe(const float3& v) {
        float len = length(v);
        if (len <= 1e-6) {
            return float3(0.0f);
        }
        return v / len;
    }

    inline constexpr float3 lerp(const float3& a, const float3& b, float t) {
        return a + (b - a) * t;
    }

    inline constexpr float3 abs(const float3& v) {
        return float3(std::fabs(v.x), std::fabs(v.y), std::fabs(v.z));
    }

    inline constexpr float3 cross(const float3& a, const float3& b) {
        return float3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

} // namespace hamu