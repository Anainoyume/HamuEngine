#include "IndexBuffer.h"

namespace hamu
{
    IndexBuffer::IndexBuffer() : id_(0), byte_size_(0), indices_size_(0) {
        glGenBuffers(1, &id_);
    }

    IndexBuffer::IndexBuffer(const unsigned int* data, size_t sizeInBytes, GLenum usage) : IndexBuffer() {
        byte_size_ = sizeInBytes;
        SetData(data, sizeInBytes, usage);
    }

    IndexBuffer::IndexBuffer(const std::vector<unsigned int>& data, GLenum usage)
        : IndexBuffer(data.data(), data.size() * sizeof(unsigned int), usage) {}

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        : id_(other.id_), byte_size_(other.byte_size_), indices_size_(other.indices_size_) {
        other.id_           = 0;
        other.byte_size_    = 0;
        other.indices_size_ = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &id_);
            id_           = other.id_;
            byte_size_    = other.byte_size_;
            indices_size_ = other.indices_size_;

            other.id_           = 0;
            other.byte_size_    = 0;
            other.indices_size_ = 0;
        }
        return *this;
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &id_);
    }

    void IndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
    }

    void IndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void IndexBuffer::SetData(const unsigned int* data, size_t sizeInBytes, GLenum usage) {
        byte_size_    = sizeInBytes;
        indices_size_ = byte_size_ / sizeof(unsigned int);
        this->Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeInBytes, data, usage);
    }

    void IndexBuffer::SetData(const std::vector<unsigned int>& data, GLenum usage) {
        SetData(data.data(), data.size() * sizeof(unsigned int), usage);
    }


} // namespace hamu