#pragma once

#include <bit>
#include <cmath>
#include <cstdint>
#include <cassert>

namespace test
{
    // IEEE754 的浮点编码在同号范围内按数值大小单调排列。
    // float_order() 将整个 float 空间映射为单调递增的 uint32_t，
    // 因此两个映射值的差就是它们的 ULP Distance。
    // Warn: 不要计算符号不同的两个浮点数, 没有意义
    inline constexpr uint32_t float_order(float x) {
        auto bits = std::bit_cast<uint32_t>(x);
        return (bits & 0x80000000u) ? (~bits) : (bits | 0x80000000u);
    }

    inline constexpr uint32_t ulp_distance(float a, float b) {
        assert(!std::isnan(a));
        assert(!std::isnan(b));
        auto bit_a = float_order(a);
        auto bit_b = float_order(b);
        return bit_a > bit_b ? bit_a - bit_b : bit_b - bit_a;
    }

} // namespace test