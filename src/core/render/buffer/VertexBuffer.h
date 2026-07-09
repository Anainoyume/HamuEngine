#pragma once

#include "glad/glad.h"
#include <vector>

namespace hamu
{

    class VertexBuffer
    {
    public:
        VertexBuffer();
        VertexBuffer(const void* data, size_t sizeInBytes, GLenum usage = GL_STATIC_DRAW);
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&)            = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        void Bind() const;
        void Unbind() const;

        void SetData(const void* data, size_t sizeInBytes, GLenum usage = GL_STATIC_DRAW);
        void Update(size_t offsetInBytes, size_t sizeInBytes, const void* data);

        template <typename Tp>
        VertexBuffer(const std::vector<Tp>& data, GLenum usage = GL_STATIC_DRAW)
            : VertexBuffer(data.data(), data.size() * sizeof(Tp), usage) {}

        template <typename Tp>
        void SetData(const std::vector<Tp>& sizeInBytes, GLenum usage = GL_STATIC_DRAW) {
            SetData(sizeInBytes.data(), sizeInBytes.size() * sizeof(Tp), usage);
        }

        GLuint Id() const noexcept { return id_; }
        size_t ByteSize() const noexcept { return byte_size_; }

    private:
        GLuint id_;
        size_t byte_size_;
    };

} // namespace hamu