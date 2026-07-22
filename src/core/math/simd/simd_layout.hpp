#pragma once

#include <cstddef>

namespace hamu::simd
{

    template <typename T, size_t Cnt, size_t Grp>
    struct simd_layout
    {
        using data_type                     = T;
        static constexpr size_t data_count  = Cnt;
        static constexpr size_t group_count = Grp;
    };

} // namespace hamu::simd