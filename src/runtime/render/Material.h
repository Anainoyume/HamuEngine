#pragma once

#include "platform/api.h"

#include "Shader.h"
#include "Texture.h"
#include "buffer/UniformBuffer.h"

#include <memory>
#include <vector>

namespace hamu
{

    struct TextureUnit
    {
        std::string name;
        std::shared_ptr<Texture> texture;
    };

    class HAMU_API Material
    {
    public:
        static Material default_material;

        Material() : id_(material_id_++) {}
        Material(const std::shared_ptr<Shader>& shader) : has_shader_(true), shader_(shader), id_(material_id_++) {};

        Material& SetShader(const std::shared_ptr<Shader>& shader);
        Material& AddTexture(const std::string_view& name, const std::shared_ptr<Texture>& texture);
        Material& SetUniformBuffer(
            const std::string_view& name, const std::shared_ptr<UniformBuffer>& uniformBuffer, size_t bindingPoint = 0
        );

        void Bind() const;

        size_t Id() const noexcept { return id_; }

        const Shader& GetShader() const noexcept;

    private:
        bool has_shader_ = false;
        mutable std::shared_ptr<Shader> shader_;

        // texture
        std::vector<TextureUnit> textures_;

        // uniforms
        std::shared_ptr<UniformBuffer> uniform_buffers_;
        size_t binding_point_ = 0;

        size_t id_;
        inline static size_t material_id_ = 0;

        bool CheckShader() const noexcept;
    };

} // namespace hamu