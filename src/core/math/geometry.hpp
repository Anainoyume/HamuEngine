#pragma once

#include "simd/simd.hpp"

#include "float4.hpp"
#include "float4x2.hpp"

#include <utility>

namespace hamu
{

    [[nodiscard]]
    inline auto dot(const float4& a, const float4& b) noexcept {
        using simd4f = simd::simd<4, float>;
        auto va      = simd4f::load<true>(&a.x);
        auto vb      = simd4f::load<true>(&b.x);
        auto sum     = va * vb;
        sum          = sum + merge_high(sum, sum);
        sum          = sum + shuffle<1, 0, 0, 0>(sum, sum);
        return lane<0>(sum);
    }

    [[nodiscard]]
    inline auto dot(const float4x2& a, const float4x2& b) noexcept {
        auto sum = a.reg * b.reg;
        sum      = sum + shuffle<2, 3, 2, 3>(sum, sum);
        sum      = sum + shuffle<1, 0, 0, 0>(sum, sum);
        return std::pair<float, float> {lane<0>(sum), lane<4>(sum)};
    }

    [[nodiscard]]
    inline auto length(const float4& a) noexcept {
        using simd4f = simd::simd<4, float>;
        auto d       = dot(a, a);
        auto x       = simd4f::boardcast(d);
        x            = sqrt(x);
        return lane<0>(x);
    }

    [[nodiscard]]
    inline auto length(const float4x2& a) noexcept {
        auto [d1, d2] = dot(a, a);
        auto x        = simd::combine(d1, d2);
        x             = sqrt(x);
        return std::pair<float, float> {lane<0>(x), lane<4>(x)};
    }

    [[nodiscard]]
    inline auto distance(const float4& a, const float4& b) noexcept {
        return length(b - a);
    }

    [[nodiscard]]
    inline auto distance(const float4x2& a, const float4x2& b) noexcept {
        return length(b - a);
    }

    [[nodiscard]]
    inline auto normalize(const float4& a) noexcept {
        using simd4f = simd::simd<4, float>;
        auto va      = simd4f::load<true>(&a.x);

        auto sum = va * va;
        sum      = sum + merge_high(sum, sum);
        sum      = sum + shuffle<1, 0, 0, 0>(sum, sum);
        auto d   = lane<0>(sum);
        auto x   = simd4f::boardcast(d);

        auto inv  = rsqrt(x);
        auto inv2 = inv * inv;
        inv       = inv * sub_mul(simd::SIMD4F_CONST_THREE_HALF, x * simd::SIMD4F_CONST_HALF, inv2);
        va        = va * inv;

        float4 result;
        va.store(&result.x);
        return result;
    }

    [[nodiscard]]
    inline auto normalize(const float4x2& a) noexcept {
        auto va  = a.reg;
        auto sum = va * va;
        sum      = sum + shuffle<2, 3, 2, 3>(sum, sum);
        sum      = sum + shuffle<1, 0, 0, 0>(sum, sum);
        auto x   = simd::combine(lane<0>(sum), lane<4>(sum));

        auto inv  = rsqrt(x);
        auto inv2 = inv * inv;
        inv       = inv * sub_mul(simd::SIMD8F_CONST_THREE_HALF, x * simd::SIMD8F_CONST_HALF, inv2);
        va        = va * inv;

        return float4x2(va);
    }

    [[nodiscard]]
    inline auto lerp(const float4& a, const float4& b, float t) noexcept {
        using simd4f = simd::simd<4, float>;
        auto va      = simd4f::load<true>(&a.x);
        auto vb      = simd4f::load<true>(&b.x);
        auto vt      = simd4f::boardcast(t);
        auto x       = mul_add(vb - va, vt, va);

        float4 result;
        x.store(&result.x);
        return result;
    }

    [[nodiscard]]
    inline auto lerp(const float4x2& a, const float4x2& b, float t) noexcept {
        using simd8f = simd::simd<8, float>;
        auto vt      = simd8f::boardcast(t);
        auto x       = mul_add(b.reg - a.reg, vt, a.reg);
        return float4x2(x);
    }

    [[nodiscard]]
    inline auto lerp(const float4x2& a, const float4x2& b, float t1, float t2) noexcept {
        auto vt = simd::combine(t1, t2);
        auto x  = mul_add(b.reg - a.reg, vt, a.reg);
        return float4x2(x);
    }

    [[nodiscard]]
    inline auto abs(const float4& a) noexcept {
        using simd4f = simd::simd<4, float>;
        auto va      = simd4f::load<true>(&a.x);
        auto x       = bit_andnot(simd::SIMD4F_CONST_NEG_ZERO, va);

        float4 result;
        x.store(&result.x);
        return result;
    }

    [[nodiscard]]
    inline auto abs(const float4x2& a) noexcept {
        auto x = bit_andnot(simd::SIMD8F_CONST_NEG_ZERO, a.reg);
        return float4x2(x);
    }

} // namespace hamu