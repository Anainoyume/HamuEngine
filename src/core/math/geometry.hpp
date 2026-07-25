#pragma once

#include "simd/simd.hpp"
#include "simd/simd_layout.hpp"

#include <type_traits>
#include <utility>

namespace hamu
{

    template <simd::simd_type T>
    inline constexpr auto dot(T a, T b) noexcept {
        using layout = T::layout_type;
        if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 4 && layout::data_packet == 1
        ) {
            using simd4f = simd::simd<4, float>;
            auto va      = simd4f::load(a.data());
            auto vb      = simd4f::load(b.data());
            auto sum     = va * vb;
            sum          = sum + merge_high(sum, sum);
            sum          = sum + shuffle<1, 0, 0, 0>(sum, sum);
            return lane<0>(sum);
        }
        else if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 8 && layout::data_packet == 2
        ) {
            using simd8f = simd::simd<8, float>;
            auto va      = simd8f::load(a.data());
            auto vb      = simd8f::load(b.data());
            auto sum     = va * vb;
            sum          = sum + shuffle<2, 3, 2, 3>(sum, sum);
            sum          = sum + shuffle<1, 0, 0, 0>(sum, sum);
            return std::pair<float, float> {lane<0>(sum), lane<4>(sum)};
        }
        else {
            static_assert(false, "The current dot function doesn't support this type of calculation.");
        }
    }

    template <simd::simd_type T>
    inline constexpr auto length(T a) noexcept {
        using layout = T::layout_type;
        if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 4 && layout::data_packet == 1
        ) {
            using simd4f = simd::simd<4, float>;
            auto va      = simd4f::load(a.data());
            auto d       = dot(va, va);
            auto x       = simd4f::boardcast(d);
            x            = sqrt(x);
            return lane<0>(x);
        }
        else if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 8 && layout::data_packet == 2
        ) {
            using simd8f  = simd::simd<8, float>;
            auto va       = simd8f::load(a.data());
            auto [d1, d2] = DotPair(va, va);
            auto x        = combine(d1, d2);
            x             = sqrt(x);
            return std::pair<float, float> {lane<0>(x), lane<4>(x)};
        }
        else {
            static_assert(false, "The current length function doesn't support this type of calculation.");
        }
    }

    template <simd::simd_type T>
    inline constexpr auto distance(T a, T b) noexcept {
        using layout = T::layout_type;
        if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 4 && layout::data_packet == 1
        ) {
            using simd4f = simd::simd<4, float>;
            auto va      = simd4f::load(a.data());
            auto vb      = simd4f::load(b.data());
            return length(vb - va);
        }
        else if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 8 && layout::data_packet == 2
        ) {
            using simd8f = simd::simd<8, float>;
            auto va      = simd8f::load(a.data());
            auto vb      = simd8f::load(b.data());
            return length(vb - va);
        }
        else {
            static_assert(false, "The current distance function doesn't support this type of calculation.");
        }
    }

    template <simd::simd_type T>
    inline constexpr auto normalize(T a) noexcept {
        using layout = T::layout_type;
        if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 4 && layout::data_packet == 1
        ) {
            using simd4f            = simd::simd<4, float>;
            auto va                 = simd4f::load(a.data());
            auto d                  = dot(va, va);
            auto x                  = simd4f::boardcast(d);
            static const auto half  = simd4f::boardcast(0.5f);
            static const auto three = simd4f::boardcast(1.5f);

            auto inv  = rsqrt(x);
            auto inv2 = inv * inv;
            inv       = inv * sub_mul(three, x * half, inv2);
            va        = va * inv;

            T result;
            va.store(result.data());
            return result;
        }
        else if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 8 && layout::data_packet == 2
        ) {
            using simd8f            = simd::simd<8, float>;
            auto va                 = simd8f::load(a.data());
            auto [d1, d2]           = dot(va, va);
            auto x                  = combine(d1, d2);
            static const auto half  = simd8f::boardcast(0.5f);
            static const auto three = simd8f::boardcast(1.5f);

            auto inv  = rsqrt(x);
            auto inv2 = inv * inv;
            inv       = inv * sub_mul(three, x * half, inv2);
            va        = va * inv;

            T result;
            va.store(result.data());
            return result;
        }
        else {
            static_assert(false, "The current normalize function doesn't support this type of calculation.");
        }
    }

    template <simd::simd_type T>
    inline constexpr auto lerp(T a, T b, typename T::data_type t) noexcept {
        using layout = T::layout_type;
        if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 4 && layout::data_packet == 1
        ) {
            using simd4f = simd::simd<4, float>;
            auto va      = simd4f::load(a.data());
            auto vb      = simd4f::load(b.data());
            auto vt      = simd4f::boardcast(t);
            auto x       = mul_add(b - a, vt, a);

            T result;
            x.store(result.data());
            return result;
        }
        else if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 8 && layout::data_packet == 1
        ) {
            using simd8f = simd::simd<8, float>;
            auto va      = simd8f::load(a.data());
            auto vb      = simd8f::load(b.data());
            auto vt      = simd8f::boardcast(t);
            auto x       = mul_add(b - a, vt, a);

            T result;
            x.store(result.data());
            return result;
        }
        else {
            static_assert(false, "The current lerp function doesn't support this type of calculation.");
        }
    }

    template <simd::simd_type T>
    inline constexpr auto abs(T a) noexcept {
        using layout = T::layout_type;
        if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 4 && layout::data_packet == 1
        ) {
            using simd4f           = simd::simd<4, float>;
            auto va                = simd4f::load(a.data());
            static const auto mask = simd4f::boardcast(-0.0f);
            auto x                 = bit_andnot(mask, va);

            T result;
            x.store(result.data());
            return result;
        }
        else if constexpr (
            std::is_same_v<typename layout::data_type, float> && layout::data_count == 8 && layout::data_packet == 2
        ) {
            using simd8f           = simd::simd<8, float>;
            auto va                = simd8f::load(a.data());
            static const auto mask = simd8f::boardcast(-0.0f);
            auto x                 = bit_andnot(mask, va);

            T result;
            x.store(result.data());
            return result;
        }
        else {
            static_assert(false, "The current abs function doesn't support this type of calculation.");
        }
    }

} // namespace hamu