#pragma once

#include "platform/api.h"

#include "Material.h"
#include "Mesh.h"
#include "runtime/transform/Transform.h"

#include <memory>
#include <vector>

namespace hamu
{

    class HAMU_API MeshRenderer
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