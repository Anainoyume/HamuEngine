#include "Renderer.h"
#include "Material.h"
#include "Shader.h"
#include "buffer/UniformBuffer.h"
#include "buffer/VertexBuffer.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui.h"
#include "core/render/Mesh.h"
#include "core/render/buffer/IndexBuffer.h"
#include "core/transform/Transform.h"
#include <GL/gl.h>
#include <algorithm>
#include <chrono>
#include <memory>

namespace hamu
{



    RenderCommand::RenderCommand(
        const Mesh* mesh, const Transform* transform, const Material* material, SubMesh submesh
    )
        : mesh(mesh), transform(transform), material(material), submesh(submesh) {
        sort_key = ((material->Id() & 0xffff) << 48) | ((mesh->Id() & 0xffff) << 32) | (submesh.index_offset & 0xffff);
    };



    // 静态成员定义, 静态数据成员必须在类外再“定义一次”，否则它不存在实体
    std::unique_ptr<UniformBuffer> Renderer::camera_ubo_;
    std::vector<RenderCommand> Renderer::render_queue_;
    RenderProfileBlock Renderer::render_profile_block_;
    std::unique_ptr<hamu::VertexBuffer> Renderer::instance_vbo_;

    glm::vec4 Renderer::clear_color_ = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    GLbitfield Renderer::clear_mask_ = 0;
    bool Renderer::prepare_success_  = false;

    void Renderer::Submit(const Mesh* mesh, SubMesh submesh, const Transform* transform, const Material* material) {
        if (!prepare_success_) return;
        render_queue_.emplace_back(mesh, transform, material, submesh);
    }

    void Renderer::ConfigureClearColor(float r, float g, float b, float a) {
        clear_color_.r = r;
        clear_color_.g = g;
        clear_color_.b = b;
        clear_color_.a = a;
    }

    void Renderer::ConfigureClearFlags(GLbitfield mask) {
        clear_mask_ = mask;
    }

    void Renderer::Init() {
        Shader::default_shader.Create(
            "resource/shaders/default/default_fallback.vert", "resource/shaders/default/default_fallback.frag"
        );

        camera_ubo_ = std::make_unique<UniformBuffer>();
        camera_ubo_->SetData(nullptr, 2 * sizeof(glm::mat4), GL_DYNAMIC_DRAW);
        camera_ubo_->BindPoint(0);

        instance_vbo_ = std::make_unique<hamu::VertexBuffer>();
        instance_vbo_->SetData(nullptr, MAX_INSTANCES * sizeof(glm::mat4), GL_DYNAMIC_DRAW);

        render_queue_.reserve(3 * MAX_INSTANCES);
        batch_transforms_.reserve(MAX_INSTANCES);
    }

    void Renderer::BeginScene(const Camera& camera) {
        glClearColor(clear_color_.r, clear_color_.g, clear_color_.b, clear_color_.a);
        glClear(clear_mask_);

        camera_ubo_->Update(0, 64, glm::value_ptr(camera.GetViewMatrix()));
        camera_ubo_->Update(64, 64, glm::value_ptr(camera.GetProjectionMatrix()));

        render_queue_.clear();

        prepare_success_ = true;
    }

    void Renderer::EndScene() {
        prepare_success_ = false;
        render_profile_block_.Reset();

        if (render_queue_.empty()) return;

#ifdef CPU_PROFILER
        auto t_render_sort_start = std::chrono::high_resolution_clock::now();
#endif

        // 排序
        std::sort(render_queue_.begin(), render_queue_.end());


#ifdef CPU_PROFILER
        auto t_render_sort_end    = std::chrono::high_resolution_clock::now();
        auto t_render_batch_start = std::chrono::high_resolution_clock::now();
#endif


        bool mesh_is_null     = true;
        bool material_is_null = true;
        size_t size           = render_queue_.size();

        for (size_t i = 0; i < size; i++) {
            const auto& cmd = render_queue_[i];

            batch_transforms_.push_back(cmd.transform->GetModelMatrix());

            bool is_last                = i == size - 1;
            bool has_different_mesh     = !is_last && cmd.mesh != render_queue_[i + 1].mesh;
            bool has_different_material = !is_last && cmd.material != render_queue_[i + 1].material;

            if (is_last) {
                cmd.mesh->Bind();
                cmd.material->Bind();

                render_profile_block_.mesh_binding_count += 1;
                render_profile_block_.material_binding_count += 1;
            }

            if (has_different_mesh) {
                cmd.mesh->Bind();
                mesh_is_null = false;
                render_profile_block_.mesh_binding_count += 1;
            }

            if (has_different_material) {
                cmd.material->Bind();
                material_is_null = false;
                render_profile_block_.material_binding_count += 1;
            }

            if (is_last || has_different_mesh || has_different_material) {

                if (mesh_is_null) {
                    cmd.mesh->Bind();
                    mesh_is_null = false;
                }

                if (material_is_null) {
                    cmd.material->Bind();
                    material_is_null = false;
                }

                DrawCall(cmd, batch_transforms_);
                batch_transforms_.clear();
            }

            // if (is_last) {
            //     cmd.mesh->Unbind();
            // }
        }

#ifdef CPU_PROFILER
        auto t_render_batch_end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> d_sort  = t_render_sort_end - t_render_sort_start;
        std::chrono::duration<double, std::milli> d_batch = t_render_batch_end - t_render_batch_start;

        ImGui::Text("Render Sort:  %.3f ms", d_sort.count());
        ImGui::Text("Render Batch: %.3f ms", d_batch.count());
#endif
    }

    void Renderer::DrawCall(const RenderCommand& cmd, const std::vector<glm::mat4>& trans) {
        // cmd.material->GetShader().SetMatrix4fv("objectToWorld", glm::value_ptr(cmd.transform->GetModelMatrix()));

        // 临时上传顶点到 vbo
        instance_vbo_->Update(0, trans.size() * sizeof(glm::mat4), trans.data());

        glDrawElementsInstanced(
            GL_TRIANGLES,
            cmd.submesh.index_count,
            GL_UNSIGNED_INT,
            (void*)(cmd.submesh.index_offset * sizeof(unsigned int)),
            trans.size() // 要绘制的实例数量, 因为此时 mesh 和 material 一定相同, 只有 trans 不同
        );

        render_profile_block_.drawcall_count += 1;
    }

} // namespace hamu