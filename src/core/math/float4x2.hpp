#pragma once

#include <utility>
#include <vector>

#include "float4.hpp"
#include "simd/simd.hpp"

namespace hamu
{
    struct float4x2
    {
    public:
        explicit float4x2(const float* data) noexcept : reg(simd::simd<8, float>::load(data)) {}

        explicit float4x2(const simd::simd<8, float>& reg) noexcept : reg(reg) {}

        void store(float* data) noexcept;

        simd::simd<8, float> reg;
    };

    inline void float4x2::store(float* data) noexcept {
        reg.store(data);
    }

    [[nodiscard]]
    inline float4x2 operator+(const float4x2& a, const float4x2& b) noexcept {
        return float4x2(a.reg + b.reg);
    }

    [[nodiscard]]
    inline float4x2 operator-(const float4x2& a, const float4x2& b) noexcept {
        return float4x2(a.reg - b.reg);
    }

    [[nodiscard]]
    inline float4x2 operator*(const float4x2& a, const float4x2& b) noexcept {
        return float4x2(a.reg * b.reg);
    }

    [[nodiscard]]
    inline float4x2 operator/(const float4x2& a, const float4x2& b) noexcept {
        return float4x2(a.reg / b.reg);
    }

    [[nodiscard]]
    inline float4x2 operator*(const float4x2& v, float s) noexcept {
        return float4x2(v.reg * s);
    }

    [[nodiscard]]
    inline float4x2 operator*(float s, const float4x2& v) noexcept {
        return float4x2(s * v.reg);
    }

    [[nodiscard]]
    inline float4x2 operator/(float s, const float4x2& v) noexcept {
        return float4x2(s / v.reg);
    }

    [[nodiscard]]
    inline float4x2 operator/(const float4x2& v, float s) noexcept {
        return float4x2(v.reg / s);
    }

    [[nodiscard]]
    inline float4x2 operator-(const float4x2& v) noexcept {
        return float4x2(simd::bit_xor(v.reg, simd::SIMD8F_CONST_NEG_ZERO));
    }

} // namespace hamu