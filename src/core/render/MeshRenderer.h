#pragma once

#include "Material.h"
#include "Mesh.h"
#include "transform/Transform.h"

#include <memory>
#include <vector>

namespace hamu
{

    class MeshRenderer
    {
    public:
        using MeshPtr     = std::shared_ptr<Mesh>;
        using MaterialPtr = std::shared_ptr<Material>;

        MeshRenderer(const MeshPtr& mesh);
        ~MeshRenderer() = default;

        void OnRender(const Transform& transform) const;

        void AddMaterial(const MaterialPtr& material);

    private:
        MeshPtr mesh_;
        std::vector<MaterialPtr> materials_;
    };

} // namespace hamu