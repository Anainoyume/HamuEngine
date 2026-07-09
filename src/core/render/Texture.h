#pragma once

#include "glad/glad.h"

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

#include "TextureDescriptor.h"

namespace hamu
{

    class Texture
    {
    public:
        Texture() = default;

        /**
         * @brief Texture 的构造函数
         * @note  内部会修改 GL_TEXTURE_2D 绑定状态为当前纹理
         * @param filePath          纹理图像文件路径
         * @param generateMipmap    是否生成多级渐远纹理
         * @param descriptor        纹理描述符, 用于指定纹理的各种属性
         */
        Texture(const fs::path& filePath, bool generateMipmap = true, const TextureDescriptor& descriptor = {});
        ~Texture();

        Texture(const Texture&)            = delete;
        Texture& operator=(const Texture&) = delete;

        Texture(Texture&& other) noexcept;
        Texture& operator=(Texture&& other) noexcept;

        void Load(const fs::path& filePath, bool generateMipmap = true, const TextureDescriptor& descriptor = {});
        void Bind(int unit = 0) const;

        GLuint Id() const noexcept { return CheckLoaded() ? id_ : 0; }
        int Width() const noexcept { return CheckLoaded() ? width_ : 0; }
        int Height() const noexcept { return CheckLoaded() ? height_ : 0; }
        int ColorChannels() const noexcept { return CheckLoaded() ? color_channels_ : 0; }
        bool IsLoaded() const noexcept { return is_loaded_; }

    private:
        GLuint id_          = 0; // 纹理ID
        int width_          = 0;
        int height_         = 0;
        int color_channels_ = 0;
        bool is_loaded_     = false;

        bool CheckLoaded() const noexcept;
    };

} // namespace hamu