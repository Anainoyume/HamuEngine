#pragma once
#include <cstddef>

namespace hamu
{
    struct SubMesh
    {
        size_t index_offset;
        size_t index_count;

        bool operator==(const SubMesh& other) const {
            return index_offset == other.index_offset && index_count == other.index_count;
        }
        bool operator<(const SubMesh& other) const {
            if (index_count != other.index_count) return index_count > other.index_count;
            return index_offset < other.index_offset;
        }
        bool operator!=(const SubMesh& other) const { return !operator==(other); }
    };
} // namespace hamu