#pragma once

#include "glad/glad.h"

#include "VertexLayout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace hamu
{

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        VertexArray(const VertexArray&)            = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void Bind() const;
        void Unbind() const;

        void AddVertexLayout(const VertexBuffer& vertexBuffer, const VertexLayout& layout);
        void SetIndexBuffer(const IndexBuffer& indexBuffer);

        GLuint Id() const noexcept { return id_; }

    private:
        GLuint id_;
        GLuint next_attrib_index_;
    };

} // namespace hamu