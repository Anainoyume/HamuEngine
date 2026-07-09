#pragma once

#include <vector>
#include "transform/Transform.h"

// clang-format off
// 立方体, 为了保持每个面的法线, 贴图正确, 最少需要 24 个点
inline std::vector cube = {
//   posx   posy   posz    u      v
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  1.0f,
        
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 
    -0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  1.0f,  1.0f,

    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  1.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  1.0f,

    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, 
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, 
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  1.0f,
};

// 立方体三角形索引
inline std::vector<unsigned int> cube_indices = {
    2,  1,  0,  1,  2,  3,
    4,  5,  6,  7,  6,  5,
    10, 9,  8,  9,  10, 11,
    12, 13, 14, 15, 14, 13,
    16, 17, 18, 19, 18, 17,
    22, 21, 20, 21, 22, 23
};
 
// inline std::vector<hamu::Transform> cube_transform;
// clang-format on