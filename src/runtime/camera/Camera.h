#pragma once

#include "platform/api.h"

#include "runtime/transform/Transform.h"
#include <memory>

namespace hamu
{

    class HAMU_API Camera
    {
    public:
        Camera()  = default;
        ~Camera();

        glm::mat4 GetViewMatrix() const noexcept;
        const glm::mat4& GetProjectionMatrix() const noexcept;


        Transform& GetTransform() noexcept { return transform_; }

        float Aspect() const noexcept { return aspect_; }
        float Fov() const noexcept { return fov_; }
        float Near() const noexcept { return near_; }
        float Far() const noexcept { return far_; }

        // Setters - 返回 bool 表示是否成功，支持链式调用
        bool SetAspect(float aspect) noexcept {
            if (aspect <= 0.001f) {
                return false;
            }
            aspect_ = aspect;
            dirty_  = true;
            return true;
        }

        bool SetAspect(float width, float height) noexcept {
            if (height <= 0.001f) {
                return false;
            }
            aspect_ = width / height;
            dirty_  = true;
            return true;
        }

        bool SetFov(float radians) noexcept {
            if (radians < 0.01f || radians > 3.14f) {
                return false;
            }
            fov_   = radians;
            dirty_ = true;
            return true;
        }

        bool SetNear(float near) noexcept {
            if (near <= 0.001f || near >= far_) {
                return false;
            }
            near_  = near;
            dirty_ = true;
            return true;
        }

        bool SetFar(float far) noexcept {
            if (far <= near_) {
                return false;
            }
            far_   = far;
            dirty_ = true;
            return true;
        }

        bool SetClipPlanes(float near, float far) noexcept {
            if (near <= 0.001f || far <= near) {
                return false;
            }
            near_  = near;
            far_   = far;
            dirty_ = true;
            return true;
        }

        void AsMainCamera() noexcept;
        static Camera* Main() noexcept;


    private:
        Transform transform_;

        inline static Camera* main_camera_ = nullptr;
        bool is_main_                      = false;

        float aspect_ = 16.0f / 9.0f;
        float fov_    = glm::radians(60.0f);
        float near_   = 0.1f;
        float far_    = 100.0f;

        mutable bool dirty_                  = true;
        mutable glm::mat4 projection_matrix_ = glm::mat4(1.0f);
    };

} // namespace hamu