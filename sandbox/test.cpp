#include <iostream>

#include "packed_float4.hpp"
#include "geometry.hpp"
#include "simd/simd_layout.hpp"

struct P : hamu::simd::simd_layout<float, 8, 2> {
    float x;

    float* data() noexcept { return &x; }
};

int main() {

    P a;
    P b;

    auto [x, y] = hamu::dot(a, b);

}