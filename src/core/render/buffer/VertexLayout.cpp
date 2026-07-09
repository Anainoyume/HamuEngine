#include "VertexLayout.h"

#include <format>

namespace hamu
{

    VertexLayout::VertexLayout() : stride_(0) {}

    void VertexLayout::Push(VertexAttribute attribute) {
        attributes_.push_back(attribute);
        stride_ += attribute.GetSize();
    }

    void VertexLayout::Push(GLint count, GLenum type, GLboolean normalized) {
        attributes_.emplace_back(count, type, normalized);
        stride_ += attributes_.back().GetSize();
    }

    void VertexLayout::PushFloat(GLint count, GLboolean normalized) {
        Push(count, GL_FLOAT, normalized);
    }

    std::ostream& operator<<(std::ostream& os, const VertexAttribute& attribute) {
        os << std::format(
            "VertexAttribute: [struct: {}×{}, normalized: {}]",
            attribute.count,
            VertexAttribute::GetTypeString(attribute.type),
            attribute.normalized ? "true" : "false"
        );
        return os;
    }

} // namespace hamu