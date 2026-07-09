#include "Mesh.h"

namespace hamu
{

    void Mesh::Bind() const {
        vao_->Bind();
    }

    void Mesh::Unbind() const {
        vao_->Unbind();
    }

    void Mesh::AddVertexLayout(const VertexBuffer& vertexBuffer, const VertexLayout& layout) {
        vao_->AddVertexLayout(vertexBuffer, layout);
    }

    void Mesh::SetIndexBuffer(const IndexBuffer& indexBuffer) {
        vao_->SetIndexBuffer(indexBuffer);
        vertices_count_ = indexBuffer.Size();
    }

    void Mesh::SetSubMeshes(const std::vector<SubMesh>& subMeshes) {
        sub_meshes_ = subMeshes;
    }

    void Mesh::SetSubMeshes(std::vector<SubMesh>&& subMeshes) {
        sub_meshes_ = std::move(subMeshes);
    }

    Mesh::Mesh() : vao_(std::make_unique<VertexArray>()), id_(mesh_id_++) {
        // 这里就固定死实例化的位置
        Bind();
        Renderer::InstanceVBO()->Bind();
        // 属性绑定到 12, 13, 14, 15
        for (size_t i = 0; i < 4; i++) {
            glEnableVertexAttribArray(12 + i);
            glVertexAttribPointer(12 + i, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(i * sizeof(glm::vec4)));

            // 核心代码
            // 0 = 每个顶点更新 (标准行为)
            // 1 = 每个实例更新 (Instancing 行为)
            glVertexAttribDivisor(12 + i, 1);
        }
    }

} // namespace hamu