#pragma once

namespace hamu
{

#define META_ID_X(x) x,

#define META_ID_COMPONENTS                                                                                             \
    META_ID_X(object)                                                                                                  \
    META_ID_X(float2)                                                                                                  \
    META_ID_X(float3)                                                                                                  \
    META_ID_X(float4)                                                                                                  \
    META_ID_X(quat)                                                                                                    \
    META_ID_X(mat3x3)                                                                                                  \
    META_ID_X(mat4x4)                                                                                                  \
    META_ID_X(transform)

    // clang-format off
    enum class meta_id : int
    {
        _invalid = -1,

        META_ID_COMPONENTS 

        _count
    };
    // clang-format on

    inline constexpr int meta_count = static_cast<int>(meta_id::_count);

#undef META_ID_X

} // namespace hamu