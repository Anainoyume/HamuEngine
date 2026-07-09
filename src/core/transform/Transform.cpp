#include "Transform.h"
#include "glm/ext/quaternion_float.hpp"

#include <iostream>
#include <vector>

namespace hamu
{

    Transform::Transform(const glm::vec3& position, const glm::quat& rotation, const glm::vec3& scale)
        : position_(position), rotation_(rotation), scale_(scale) {}

    const glm::mat4& Transform::GetModelMatrix() const {
        if (is_dirty_) {
            cached_matrix_ = glm::mat4(1.0f);
            // 注意顺序：先平移，再旋转，再缩放 (T * R * S) -> 先作用缩放给向量, 然后是旋转, 然后是位移
            cached_matrix_ = glm::translate(cached_matrix_, position_);
            cached_matrix_ = cached_matrix_ * glm::mat4_cast(rotation_); // 四元数转矩阵
            cached_matrix_ = glm::scale(cached_matrix_, scale_);
            is_dirty_      = false;
        }
        return cached_matrix_;
    }

    glm::vec3 Transform::Forward() const noexcept {
        // opengl 的默认前方向是负z轴
        return rotation_ * glm::vec3(0.0f, 0.0f, -1.0f);
    }

    glm::vec3 Transform::Up() const noexcept {
        return rotation_ * glm::vec3(0.0f, 1.0f, 0.0f);
    }

    glm::vec3 Transform::Right() const noexcept {
        return rotation_ * glm::vec3(1.0f, 0.0f, 0.0f);
    }

    namespace utils
    {
        void serde(JsonWriter& w, const glm::vec3& val) noexcept {
            std::vector<float> vec {val.x, val.y, val.z};
            serde(w, vec);
        }

        void deserde(JsonNode root, glm::vec3& object) noexcept {
            std::vector<float> vec;
            deserde(root, vec);
            object.x = vec[0];
            object.y = vec[1];
            object.z = vec[2];
        }

        void serde(JsonWriter& w, const glm::quat& val) noexcept {
            std::vector<float> vec {val.x, val.y, val.z, val.w};
            serde(w, vec);
        }

        void deserde(JsonNode root, glm::quat& object) noexcept {
            std::vector<float> vec;
            deserde(root, vec);
            object.x = vec[0];
            object.y = vec[1];
            object.z = vec[2];
            object.w = vec[3];
        }

        void serde(JsonWriter& w, const hamu::Transform& val) noexcept {
            ObjectSerdeHelper o(w);
            o.field("position", val.getPosition());
            o.field("rotation", val.getRotation());
            o.field("scale", val.getScale());
        }

        void deserde(JsonNode root, hamu::Transform& object) noexcept {
            glm::vec3 pos;
            glm::vec3 scale;
            glm::quat rot;

            deserde(root["position"], pos);
            deserde(root["rotation"], scale);
            deserde(root["scale"], rot);

            object.setPosition(pos);
            object.setRotation(rot);
            object.setScale(scale);
        }
    } // namespace utils

} // namespace hamu