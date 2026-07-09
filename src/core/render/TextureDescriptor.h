#pragma once

#include "glad/glad.h"

namespace hamu
{
    struct TextureDescriptor
    {
        int required_channels  = 0;                       // 需要的图像通道数, 0 表示使用原图通道数
        GLenum warp_s          = GL_CLAMP_TO_EDGE;        // 纹理环绕方式, x 方向
        GLenum warp_t          = GL_CLAMP_TO_EDGE;        // 纹理环绕方式, y 方向
        GLenum min_filter      = GL_LINEAR_MIPMAP_LINEAR; // 缩小过滤器
        GLenum mag_filter      = GL_LINEAR;               // 放大过滤器
        GLenum internal_format = GL_RGB;                  // GPU 纹理储存格式
        GLenum format          = GL_RGB;                  // 原图格式
    };

} // namespace hamu