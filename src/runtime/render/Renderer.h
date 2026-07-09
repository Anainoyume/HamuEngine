#pragma once

#include "platform/api.h"

#include "buffer/VertexBuffer.h"
#include "glad/glad.h"

#include "runtime/application/Application.h"
#include "buffer/UniformBuffer.h"
#include "runtime/render/buffer/IndexBuffer.h"
#include "runtime/transform/Transform.h"
#include "runtime/camera/Camera.h"
#include "Material.h"
#include "Mesh.h"

#include <cstddef>
#include <cstdint>
#include <memory>
#include <ostream>
#include <vector>

namespace hamu
{
    class Mesh;

    struct HAMU_API RenderCommand
    {
        uint64_t sort_key;

        const Mesh* mesh           = nullptr;
        const Transform* transform = nullptr; // 注意：排序通常不需要对比 Transform，除非你想按距离排序
        const Material* material   = nullptr;
        SubMesh submesh;

        // 1. 删除所有手写的构造/移动/赋值函数，使用 Rule of Zero
        // 裸指针拷贝开销极小（就是拷贝一个整数），不需要手动置空
        RenderCommand() = delete;

        // 自定义构造函数方便使用
        RenderCommand(const Mesh* mesh, const Transform* transform, const Material* material, SubMesh submesh);

        // 2. 修复 operator<
        bool operator<(const RenderCommand& other) const { return sort_key < other.sort_key; }
    };

    struct HAMU_API RenderProfileBlock
    {
        size_t drawcall_count         = 0;
        size_t mesh_binding_count     = 0;
        size_t material_binding_count = 0;

        void Reset() {
            drawcall_count         = 0;
            mesh_binding_count     = 0;
            material_binding_count = 0;
        }

        friend std::ostream& operator<<(std::ostream& os, const RenderProfileBlock& block) {
            os << "RenderProfileBlock{"
               << " drawcall_count: " << block.drawcall_count << ","
               << " mesh_binding_count: " << block.mesh_binding_count << ","
               << " material_binding_count: " << block.material_binding_count << " }";
            return os;
        }
    };

    class HAMU_API Renderer
    {
    public:
        static void Submit(const Mesh* mesh, SubMesh submesh, const Transform* transform, const Material* materials);

        static void ConfigureClearColor(float r, float g, float b, float a);
        static void ConfigureClearFlags(GLbitfield mask);

        static const RenderProfileBlock& ProfileBlock() noexcept { return render_profile_block_; }
        static const VertexBuffer* InstanceVBO() noexcept { return instance_vbo_.get(); }

    private:
        friend class Application; // Application 可以调用 Renderer 的私有方法

        Renderer() = delete;

        static void Init();
        static void BeginScene(const Camera& camera);
        static void EndScene();

        static void DrawCall(const RenderCommand& cmd, const std::vector<glm::mat4>& trans);

        static std::unique_ptr<UniformBuffer> camera_ubo_;
        static std::vector<RenderCommand> render_queue_;
        inline static std::vector<glm::mat4> batch_transforms_;

        static glm::vec4 clear_color_;
        static GLbitfield clear_mask_;

        static bool prepare_success_;

        // transform vbo - 必须延迟初始化
        static std::unique_ptr<VertexBuffer> instance_vbo_;
        constexpr static size_t MAX_INSTANCES = 1000;

        // profiler
        static RenderProfileBlock render_profile_block_;
    };

} // namespace hamu