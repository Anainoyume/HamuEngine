#include "Camera.h"

#include "glm/ext/matrix_transform.hpp"

namespace hamu
{

    glm::mat4 Camera::GetViewMatrix() const noexcept {
        // 第一个参数是 相机位置
        // 第二个参数是 目标位置
        // 第三个参数是 world up 向量，一般设为 y 轴正方向
        return glm::lookAt(
            transform_.getPosition(), transform_.getPosition() + transform_.Forward(), glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }

    const glm::mat4& Camera::GetProjectionMatrix() const noexcept {
        if (dirty_) {
            // 这里可以做一些缓存优化
            projection_matrix_ = glm::perspective(fov_, aspect_, near_, far_);
            dirty_             = false;
        }
        return projection_matrix_;
    }

    void Camera::AsMainCamera() noexcept {
        if (main_camera_ == this) return;
        if (main_camera_ != nullptr) {
            main_camera_->is_main_ = false;
        }
        is_main_     = true;
        main_camera_ = this;
    }

    Camera* Camera::Main() noexcept {
        return main_camera_;
    }

    Camera::~Camera() {
        if (main_camera_ == this) {
            main_camera_ = nullptr;
        }
    };

} // namespace hamu