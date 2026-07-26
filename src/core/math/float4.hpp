#pragma once

namespace hamu
{
    struct alignas(16) float4
    {
        float x, y, z, w;

        float4(float x, float y, float z, float w) noexcept : x(x), y(y), z(z), w(w) {}

        explicit float4(float k) noexcept : float4(k, k, k, k) {}

        float4() noexcept : float4(0) {}
    };

    [[nodiscard]]
    inline constexpr float4 operator+(const float4& a, const float4& b) noexcept {
        return float4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
    }

    [[nodiscard]]
    inline constexpr float4 operator-(const float4& a, const float4& b) noexcept {
        return float4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
    }

    [[nodiscard]]
    inline constexpr float4 operator*(const float4& a, const float4& b) noexcept {
        return float4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
    }

    [[nodiscard]]
    inline constexpr float4 operator/(const float4& a, const float4& b) noexcept {
        return float4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
    }

    [[nodiscard]]
    inline constexpr float4 operator*(const float4& v, float s) noexcept {
        return float4(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    [[nodiscard]]
    inline constexpr float4 operator*(float s, const float4& v) noexcept {
        return float4(v.x * s, v.y * s, v.z * s, v.w * s);
    }

    [[nodiscard]]
    inline constexpr float4 operator/(float s, const float4& v) noexcept {
        return float4(s / v.x, s / v.y, s / v.z, s / v.w);
    }

    [[nodiscard]]
    inline constexpr float4 operator/(const float4& v, float s) noexcept {
        return float4(v.x / s, v.y / s, v.z / s, v.w / s);
    }

    [[nodiscard]]
    inline constexpr float4 operator-(const float4& v) noexcept {
        return float4(-v.x, -v.y, -v.z, -v.w);
    }

} // namespace hamu