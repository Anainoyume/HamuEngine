#pragma once

#include <string_view>
#include <vector>

#include "meta_id.h"

namespace hamu
{

    struct meta_info_base
    {
        meta_info_base* parent;
        meta_id id;
    };

    template <typename T>
    struct meta_info : public meta_info_base
    {};

} // namespace hamu