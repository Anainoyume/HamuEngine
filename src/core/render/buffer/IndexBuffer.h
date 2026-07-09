#pragma once

#include "glad/glad.h"
#include <vector>

namespace hamu
{

    class IndexBuffer
    {
    public:
        IndexBuffer();
        IndexBuffer(const unsigned int* data, size_t sizeInBytes, GLenum usage = GL_STATIC_DRAW);
        IndexBuffer(const std::vector<unsigned int>& data, GLenum usage = GL_STATIC_DRAW);
        ~IndexBuffer();

        IndexBuffer(const IndexBuffer&)            = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        IndexBuffer(IndexBuffer&& other) noexcept;
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        void Bind() const;
        void Unbind() const;

        void SetData(const unsigned int* data, size_t sizeInBytes, GLenum usage = GL_STATIC_DRAW);
        void SetData(const std::vector<unsigned int>& data, GLenum usage = GL_STATIC_DRAW);

        GLuint Id() const noexcept { return id_; }
        size_t Size() const noexcept { return indices_size_; }
        size_t ByteSize() const noexcept { return byte_size_; }

    private:
        GLuint id_;
        size_t byte_size_;
        size_t indices_size_;
    };

} // namespace hamu