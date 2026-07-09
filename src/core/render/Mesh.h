#pragma once

#include "data_structure/SubMesh.h"
#include "buffer/VertexArray.h"
#include "glm/ext/vector_float4.hpp"
#include "render/Renderer.h"

#include <memory>
#include <vector>

namespace hamu
{
    // 目前的 Mesh 还只是一个 VAO 的简单封装, 后续会添加更多功能
    class Mesh
    {
    public:
        Mesh();
        ~Mesh() = default;

        Mesh(const Mesh&)            = delete;
        Mesh& operator=(const Mesh&) = delete;

        void Bind() const;
        void Unbind() const;

        size_t VerticesCount() const noexcept { return vertices_count_; }

        void AddVertexLayout(const VertexBuffer& vertexBuffer, const VertexLayout& layout);
        void SetIndexBuffer(const IndexBuffer& indexBuffer);

        void SetSubMeshes(const std::vector<SubMesh>& subMeshes);
        void SetSubMeshes(std::vector<SubMesh>&& subMeshes);

        const VertexArray& GetVertexArray() const noexcept { return *vao_; }
        const std::vector<SubMesh>& GetSubMeshes() const noexcept { return sub_meshes_; }

        size_t Id() const noexcept { return id_; }

    private:
        std::unique_ptr<VertexArray> vao_;
        std::vector<SubMesh> sub_meshes_;

        size_t vertices_count_        = 0;
        size_t id_                    = 0;
        inline static size_t mesh_id_ = 0;
    };

} // namespace hamu