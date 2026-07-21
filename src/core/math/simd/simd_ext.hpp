#pragma once

#include "simd.hpp"
#include <utility>

namespace hamu::simd
{
    using pairff = std::pair<float, float>;

    inline float Dot(simd<4, float> a, simd<4, float> b) {
        auto sum = a * b;
        sum      = sum + merge_high(sum, sum);
        sum      = sum + shuffle<1, 0, 0, 0>(sum, sum);
        return lane<0>(sum);
    }

    inline pairff DotPair(simd<8, float> a, simd<8, float> b) {
        auto sum = a * b;
        sum      = sum + shuffle<2, 3, 2, 3>(sum, sum);
        sum      = sum + shuffle<1, 0, 0, 0>(sum, sum);
        return pairff {lane<0>(sum), lane<4>(sum)};
    }

    inline float Length(simd<4, float> v) {
        using simd4f = simd<4, float>;
        auto d       = Dot(v, v);
        auto x       = simd4f::boardcast(d);
        x            = sqrt(x);
        return lane<0>(x);
    }

    inline pairff LengthPair(simd<8, float> v) {
        auto [d1, d2] = DotPair(v, v);
        auto x        = combine(d1, d2);
        x             = sqrt(x);
        return pairff {lane<0>(x), lane<4>(x)};
    }

    inline float Distance(simd<4, float> a, simd<4, float> b) {
        return Length(b - a);
    }

    inline pairff DistancePair(simd<8, float> a, simd<8, float> b) {
        return LengthPair(b - a);
    }

    inline simd<4, float> Normalize(simd<4, float> v) {
        using simd4f            = simd<4, float>;
        auto d                  = Dot(v, v);
        auto x                  = simd4f::boardcast(d);
        static const auto half  = simd4f::boardcast(0.5f);
        static const auto three = simd4f::boardcast(1.5f);

        auto inv  = rsqrt(x);
        auto inv2 = inv * inv;
        inv       = inv * sub_mul(three, x * half, inv2);
        v         = v * inv;

        return v;
    }

    inline simd<8, float> Normalize(simd<8, float> v) {
        using simd8f            = simd<8, float>;
        auto [d1, d2]           = DotPair(v, v);
        auto x                  = combine(d1, d2);
        static const auto half  = simd8f::boardcast(0.5f);
        static const auto three = simd8f::boardcast(1.5f);

        auto inv  = rsqrt(x);
        auto inv2 = inv * inv;
        inv       = inv * sub_mul(three, x * half, inv2);
        v         = v * inv;

        return v;
    }

    inline simd<4, float> Lerp(simd<4, float> a, simd<4, float> b, float t) {
        using simd4f = simd<4, float>;
        auto vt      = simd4f::boardcast(t);
        auto x       = mul_add(b - a, vt, a);
        return x;
    }

    inline simd<8, float> Lerp(simd<8, float> a, simd<8, float> b, float t) {
        using simd8f = simd<8, float>;
        auto vt      = simd8f::boardcast(t);
        auto x       = mul_add(b - a, vt, a);
        return x;
    }

    inline simd<8, float> Lerp(simd<8, float> a, simd<8, float> b, float t1, float t2) {
        auto vt      = combine(t1, t2);
        auto x       = mul_add(b - a, vt, a);
        return x;
    }

    inline simd<4, float> Abs(simd<4, float> v) {
        using simd4f           = simd<4, float>;
        static const auto mask = simd4f::boardcast(-0.0f);
        auto x                 = bit_andnot(mask, v);
        return x;
    }

    inline simd<8, float> Abs(simd<8, float> v) {
        using simd8f           = simd<8, float>;
        static const auto mask = simd8f::boardcast(-0.0f);
        auto x                 = bit_andnot(mask, v);
        return x;
    }

} // namespace hamu::simd