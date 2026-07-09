#pragma once

#include "glad/glad.h"

#include <cstddef>
#include <iostream>
#include <string>
#include <filesystem>
#include <unordered_map>
namespace fs = std::filesystem;

namespace hamu
{

    class Shader
    {
    public:
        Shader()  = default;
        ~Shader() = default;
        Shader(const fs::path& vertexPath, const fs::path& fragmentPath);

        Shader(const Shader&)            = delete;
        Shader& operator=(const Shader&) = delete;

        static Shader default_shader;

        void Use();
        void Create(const fs::path& vertexPath, const fs::path& fragmentPath);

        // uniform工具函数
        void SetBool(const std::string_view& name, bool value) const;
        void SetInt(const std::string_view& name, int value) const;
        void SetFloat(const std::string_view& name, float value) const;
        void SetMatrix4fv(
            const std::string_view& name, const float* value, size_t count = 1, bool transpose = false
        ) const;

        // 设置 uniform block 绑定点
        void SetUniformBlockBinding(const std::string_view& name, size_t bindingPoint = 0) const;

        GLuint Id() const noexcept { return id_; }

    private:
        // 程序ID
        GLuint id_       = 0;
        bool is_created_ = false;

        mutable std::unordered_map<std::string, GLuint> uniform_cache_;

        GLint GetUniformLocation(const std::string_view& name) const;
        GLint GetUniformBlockIndex(const std::string_view& name) const;
        bool CheckLinked() const noexcept;
    };



} // namespace hamu