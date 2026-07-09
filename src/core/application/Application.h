#pragma once

#include "GLFW/glfw3.h"

#include "behavior/Behavior.h"
#include "camera/Camera.h"

#include <iostream>
#include <memory>
#include <vector>

namespace hamu
{

    class Application
    {
    public:
        using WindowDeleter = void (*)(GLFWwindow*);

        Application(const std::string_view& title = "Default Title", float width = 800.0f, float height = 600.0f)
            : window_(nullptr, DeleteWindow), window_width_(width), window_height_(height) {
            if (!InitializeApplication(width, height, title)) {
                throw std::runtime_error("Failed to initialize application.");
            }
        }

        virtual ~Application();

        Application(const Application&)            = delete;
        Application& operator=(const Application&) = delete;

        void Run();
        void Close();

        GLFWwindow* Window() const noexcept { return window_.get(); }
        float Width() const noexcept { return window_width_; }
        float Height() const noexcept { return window_height_; }
        float Scale() const noexcept { return main_scale_; }
        float PhysicalWidth() const noexcept { return window_width_ * main_scale_; }
        float PhysicalHeight() const noexcept { return window_height_ * main_scale_; }
        float DeltaTime() const noexcept { return deltaTime_; }

        void SetWidth(float width) noexcept { window_width_ = width; }
        void SetHeight(float height) noexcept { window_height_ = height; }


        template <typename BehaviorType, typename... Args>
        void AddBehavior(Args&&... args) {
            // 静态断言确保 BehaviorType 继承自 Behavior
            static_assert(std::is_base_of<Behavior, BehaviorType>::value, "BehaviorType must derive from Behavior");
            behaviors_.emplace_back(std::make_unique<BehaviorType>(std::forward<Args>(args)...));
        }

    private:
        std::unique_ptr<GLFWwindow, WindowDeleter> window_;
        float window_width_  = 800.0f;
        float window_height_ = 600.0f;
        float main_scale_    = 1.0f;

        float deltaTime_ = 0.0f;

        // 为了使用多态, 我们存储 Behavior 的智能指针
        std::vector<std::unique_ptr<Behavior>> behaviors_;

        bool InitializeApplication(
            float width = 800.0f, float height = 600.0f, const std::string_view& title = "Default Title"
        );

        static void DeleteWindow(GLFWwindow* window);

        static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    };

} // namespace hamu