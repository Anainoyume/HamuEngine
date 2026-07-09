#include "UniformBuffer.h"
#include "VertexArray.h"

namespace hamu
{

    UniformBuffer::UniformBuffer() : id_(0) {
        glGenBuffers(1, &id_);
    }

    UniformBuffer::~UniformBuffer() {
        glDeleteBuffers(1, &id_);
    }

    UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept : id_(other.id_) {
        other.id_ = 0;
    }

    UniformBuffer& UniformBuffer::operator=(UniformBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &id_);
            id_       = other.id_;
            other.id_ = 0;
        }
        return *this;
    }

    void UniformBuffer::SetData(const void* data, size_t sizeInBytes, GLenum usage) {
        Bind();
        glBufferData(GL_UNIFORM_BUFFER, sizeInBytes, data, usage);
    }

    void UniformBuffer::BindPoint(int index) const {
        Bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, index, id_);
    }

    void UniformBuffer::Update(size_t offsetInBytes, size_t sizeInBytes, const void* data) {
        Bind();
        glBufferSubData(GL_UNIFORM_BUFFER, offsetInBytes, sizeInBytes, data);
    }

    void UniformBuffer::Bind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, id_);
    }

    void UniformBuffer::Unbind() const {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

} // namespace hamu