#pragma once

#include "glad/glad.h"

namespace hamu
{

    class UniformBuffer
    {
    public:
        UniformBuffer();
        ~UniformBuffer();

        UniformBuffer(const UniformBuffer&)            = delete;
        UniformBuffer& operator=(const UniformBuffer&) = delete;

        UniformBuffer(UniformBuffer&& other) noexcept;
        UniformBuffer& operator=(UniformBuffer&& other) noexcept;

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, size_t sizeInBytes, GLenum usage = GL_STATIC_DRAW);
        void BindPoint(int index = 0) const;
        void Update(size_t offsetInBytes, size_t sizeInBytes, const void* data);

        GLuint Id() const noexcept { return id_; }

    private:
        GLuint id_;
    };

} // namespace hamu