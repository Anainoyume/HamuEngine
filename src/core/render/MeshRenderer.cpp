#include "MeshRenderer.h"
#include "render/Renderer.h"

namespace hamu
{

    hamu::MeshRenderer::MeshRenderer(const MeshPtr& mesh) : mesh_(mesh) {}

    void MeshRenderer::OnRender(const Transform& transform) const {
        auto size = mesh_->GetSubMeshes().size();
        if (size == 0) {
            Renderer::Submit(
                mesh_.get(),
                {0, mesh_->VerticesCount()},
                &transform,
                materials_.empty() ? &Material::default_material : materials_[0].get()
            );
            return;
        }

        for (size_t i = 0; i < size; i++) {
            const auto& sub_mesh = mesh_->GetSubMeshes()[i];
            Renderer::Submit(
                mesh_.get(),
                sub_mesh,
                &transform,
                i < materials_.size() ? materials_[i].get() : &Material::default_material
            );
        }
    }

    void MeshRenderer::AddMaterial(const MaterialPtr& material) {
        materials_.push_back(material);
    }

} // namespace hamu
