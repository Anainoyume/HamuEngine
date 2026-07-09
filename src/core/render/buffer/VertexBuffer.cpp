#include "VertexBuffer.h"

namespace hamu
{
    VertexBuffer::VertexBuffer() : id_(0), byte_size_(0) {
        glGenBuffers(1, &id_);
    }

    VertexBuffer::VertexBuffer(const void* data, size_t sizeInBytes, GLenum usage) : VertexBuffer() {
        byte_size_ = sizeInBytes;
        SetData(data, sizeInBytes, usage);
    }

    // 列表初始化少了一个指令调用, 避免了先隐式初始化, 再赋值的过程
    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept : id_(other.id_), byte_size_(other.byte_size_) {
        other.id_        = 0;
        other.byte_size_ = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &id_);
            id_        = other.id_;
            byte_size_ = other.byte_size_;

            other.id_        = 0;
            other.byte_size_ = 0;
        }
        return *this;
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &id_);
    }

    void VertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, id_);
    }

    void VertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::SetData(const void* data, size_t sizeInBytes, GLenum usage) {
        byte_size_ = sizeInBytes;
        this->Bind();
        glBufferData(GL_ARRAY_BUFFER, sizeInBytes, data, usage);
    }

    void VertexBuffer::Update(size_t offsetInBytes, size_t sizeInBytes, const void* data) {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, offsetInBytes, sizeInBytes, data);
    }


} // namespace hamu