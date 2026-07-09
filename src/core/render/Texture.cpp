#include <iostream>

#include "Texture.h"

// 防止 ODR 违规, 只需在一个 .cpp 文件生成实现即可, 其他只需要引入头文件声明
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace hamu
{

    Texture::Texture(const fs::path& filePath, bool generateMipmap, const TextureDescriptor& descriptor) {
        Load(filePath, generateMipmap, descriptor);
    }

    Texture::~Texture() {
        if (id_ != 0) {
            glDeleteTextures(1, &id_);
        }
    }

    Texture::Texture(Texture&& other) noexcept
        : id_(other.id_), width_(other.width_), height_(other.height_), color_channels_(other.color_channels_) {

        other.id_             = 0;
        other.width_          = 0;
        other.height_         = 0;
        other.color_channels_ = 0;
    }

    // other 赋值给 this, 和移动构造的区别在于: 移动构造是新创建一个对象, 而移动赋值是将已有对象的资源释放后再进行赋值
    Texture& Texture::operator=(Texture&& other) noexcept {
        if (this != &other) {
            glDeleteTextures(1, &id_);
            this->id_             = other.id_;
            this->width_          = other.width_;
            this->height_         = other.height_;
            this->color_channels_ = other.color_channels_;

            other.id_             = 0;
            other.width_          = 0;
            other.height_         = 0;
            other.color_channels_ = 0;
        }
        return *this;
    }

    void Texture::Load(const fs::path& filePath, bool generateMipmap, const TextureDescriptor& descriptor) {
        // 翻转y轴, 因为OpenGL的纹理坐标原点在左下角, 而大多数图像的原点在左上角
        stbi_set_flip_vertically_on_load(true);

        // stbi_load 的最后一个参数 req_comp 的作用是希望获得的通道数, stb_image 会将原图自行拓展匹配至指定通道数
        std::uint8_t* image_data =
            stbi_load(filePath.string().c_str(), &width_, &height_, &color_channels_, descriptor.required_channels);

        // 如果纹理还没有被创建, 则生成一个新的纹理对象
        if (!is_loaded_) {
            glGenTextures(1, &id_);
        }
        glBindTexture(GL_TEXTURE_2D, id_);

        // 设置纹理的属性
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, descriptor.warp_s);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, descriptor.warp_t);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, descriptor.min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, descriptor.mag_filter);

        if (image_data) {
            glTexImage2D(
                GL_TEXTURE_2D,
                0,                          // 多级渐远级别 如果我们不使用 glGenerateMipmap, 则必须自己指定每一个级别
                descriptor.internal_format, // 希望把纹理储存为何种格式
                width_,                     // 宽度
                height_,                    // 高度
                0,                          // Always zero, 历史遗留问题
                descriptor.format,          // 原图格式
                GL_UNSIGNED_BYTE,           // 原图数据类型
                image_data                  // 字节数据
            );

            if (generateMipmap) glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            std::cerr << "[Error] Failed to load texture" << '\n';
        }

        // 释放图像内存
        stbi_image_free(image_data);
        is_loaded_ = true;
    }

    // 会修改激活纹理单元
    void Texture::Bind(int unit) const {
        if (!CheckLoaded()) return;
        if (unit < 0 || unit >= 32) {
            std::cerr << "[Warning] Texture unit " << unit << " is out of range [0, 31]" << '\n';
            return;
        }
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id_);
    }


    bool Texture::CheckLoaded() const noexcept {
        if (!is_loaded_) {
            std::cerr << "Error: Texture not loaded." << std::endl;
            return false;
        }
        return true;
    }

} // namespace hamu