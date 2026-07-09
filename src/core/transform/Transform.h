#pragma once

// 1. 只包含定义成员变量所需的最小头文件
#include "glm/ext/vector_float3.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

#include "tiny_json/JsonNode.h"
#include "tiny_json/JsonSerde.h"
#include "tiny_json/JsonWriter.h"

namespace hamu
{
    class Transform
    {
    public:
        Transform()  = default;
        ~Transform() = default;

        Transform(
            const glm::vec3& position, const glm::quat& rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
            const glm::vec3& scale = glm::vec3(1.0f)
        );

        const glm::mat4& GetModelMatrix() const;
        glm::vec3 Forward() const noexcept;
        glm::vec3 Up() const noexcept;
        glm::vec3 Right() const noexcept;

        const glm::vec3& getPosition() const noexcept { return position_; }
        void setPosition(const glm::vec3& position) noexcept {
            position_ = position;
            is_dirty_ = true;
        }

        const glm::quat& getRotation() const noexcept { return rotation_; }
        void setRotation(const glm::quat& rotation) noexcept {
            rotation_ = rotation;
            is_dirty_ = true;
        }

        const glm::vec3& getScale() const noexcept { return scale_; }
        void setScale(const glm::vec3& scale) noexcept {
            scale_    = scale;
            is_dirty_ = true;
        }

    private:
        glm::vec3 position_ = glm::vec3();
        glm::quat rotation_ = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 scale_    = glm::vec3(1.0f);

        mutable glm::mat4 cached_matrix_;
        mutable bool is_dirty_ = true;
    };

} // namespace hamu

namespace hamu::utils
{
    void serde(JsonWriter& w, const hamu::Transform& val) noexcept;
    void deserde(JsonNode root, hamu::Transform& object) noexcept;
} // namespace hamu::utils