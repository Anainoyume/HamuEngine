#pragma once

#include <cmath>
#include <immintrin.h>

#include "float4.hpp"
#include "simd/simd.hpp"
#include "simd/simd_ext.hpp"
#include "simd/simd_layout.hpp"

namespace hamu
{
    struct alignas(32) packed_float4 : public simd::simd_layout<float, 8, 2>
    {
        float4 lower;
        float4 upper;
         
        float* data() noexcept { return &lower.x; }
    };

} // namespace hamu