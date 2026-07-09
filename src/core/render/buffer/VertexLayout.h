#pragma once

#include "glad/glad.h"

#include <ostream>
#include <string_view>
#include <vector>

namespace hamu
{

    struct VertexAttribute
    {
        GLint count;
        GLenum type;
        GLboolean normalized = GL_FALSE;

        static constexpr size_t GetSizeOfType(GLenum type) {
            switch (type) {
                case GL_FLOAT: return sizeof(GLfloat);
                case GL_UNSIGNED_INT: return sizeof(GLuint);
                case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
                case GL_INT: return sizeof(GLint);
                case GL_BYTE: return sizeof(GLbyte);
                default: return 0;
            }
        }

        static constexpr std::string_view GetTypeString(GLenum type) {
            switch (type) {
                case GL_FLOAT: return "float";
                case GL_UNSIGNED_INT: return "uint";
                case GL_UNSIGNED_BYTE: return "ubyte";
                case GL_INT: return "int";
                case GL_BYTE: return "byte";
                default: return "unknown";
            }
        }

        size_t GetSize() const { return count * GetSizeOfType(type); }

        friend std::ostream& operator<<(std::ostream& os, const VertexAttribute& attribute);
    };

    class VertexLayout
    {
    public:
        VertexLayout();

        void Push(VertexAttribute attribute);
        void Push(GLint count, GLenum type, GLboolean normalized = GL_FALSE);

        void PushFloat(GLint count, GLboolean normalized = GL_FALSE);

        const std::vector<VertexAttribute>& GetAttributes() const noexcept { return attributes_; }
        size_t Stride() const noexcept { return stride_; }

    private:
        std::vector<VertexAttribute> attributes_;
        size_t stride_;
    };

} // namespace hamu