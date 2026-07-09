#include "VertexArray.h"
#include "VertexLayout.h"
#include <vector>

namespace hamu
{

    VertexArray::VertexArray() : id_(0), next_attrib_index_(0) {
        glGenVertexArrays(1, &id_);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &id_);
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept
        : id_(other.id_), next_attrib_index_(other.next_attrib_index_) {

        other.id_                = 0;
        other.next_attrib_index_ = 0;
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            glDeleteVertexArrays(1, &id_);
            id_                = other.id_;
            next_attrib_index_ = other.next_attrib_index_;

            other.id_                = 0;
            other.next_attrib_index_ = 0;
        }
        return *this;
    }

    void VertexArray::Bind() const {
        glBindVertexArray(id_);
    }

    void VertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::AddVertexLayout(const VertexBuffer& vertexBuffer, const VertexLayout& layout) {
        Bind();
        vertexBuffer.Bind();

        const auto& attributes = layout.GetAttributes();
        size_t offset          = 0;

        for (const auto& attr : attributes) {
            glEnableVertexAttribArray(next_attrib_index_);
            glVertexAttribPointer(
                next_attrib_index_,
                attr.count,
                attr.type,
                attr.normalized,
                static_cast<GLsizei>(layout.Stride()),
                reinterpret_cast<const void*>(offset)
            );

            offset += attr.GetSize();
            next_attrib_index_ += 1;
        }
    }

    void VertexArray::SetIndexBuffer(const IndexBuffer& indexBuffer) {
        Bind();
        indexBuffer.Bind();
    }

} // namespace hamu