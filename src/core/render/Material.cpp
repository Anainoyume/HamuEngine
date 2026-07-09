#include "Material.h"
#include "core/render/buffer/IndexBuffer.h"
#include <iostream>

namespace hamu
{

    Material Material::default_material;

    bool Material::CheckShader() const noexcept {
        if (has_shader_) return true;
        std::cerr << "Material has no associated shader." << std::endl;
        return false;
    }

    Material& Material::SetShader(const std::shared_ptr<Shader>& shader) {
        shader_     = shader;
        has_shader_ = true;
        return *this;
    }

    Material& Material::AddTexture(const std::string_view& name, const std::shared_ptr<Texture>& texture) {
        textures_.emplace_back(TextureUnit {name.data(), texture});
        return *this;
    }

    Material& Material::SetUniformBuffer(
        const std::string_view& name, const std::shared_ptr<UniformBuffer>& uniformBuffer, size_t bindingPoint
    ) {
        if (!CheckShader()) return *this;
        uniform_buffers_ = uniformBuffer;
        binding_point_   = bindingPoint;
        shader_->SetUniformBlockBinding(name, bindingPoint);
        return *this;
    }

    void Material::Bind() const {
        if (shader_ == nullptr) {
            Shader::default_shader.Use();
        }
        else {
            shader_->Use();
        }

        auto unit = 0;
        for (const auto& textureUnit : textures_) {
            textureUnit.texture->Bind(unit);
            shader_->SetInt(textureUnit.name, unit);
            unit += 1;
        }

        if (uniform_buffers_) {
            uniform_buffers_->BindPoint(binding_point_);
        }
    }

    const Shader& Material::GetShader() const noexcept {
        return CheckShader() ? *shader_ : Shader::default_shader;
    }

} // namespace hamu
