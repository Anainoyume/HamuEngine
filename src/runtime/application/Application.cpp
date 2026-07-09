#include "glad/glad.h"
#include "Application.h"

// Imgui includes
#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"
#include "runtime/render/Renderer.h"

#include <iostream>

namespace hamu
{

    Application::~Application() {

        // 销毁行为组件
        for (const auto& behavior : behaviors_) {
            behavior->Destroy();
        }

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
    }

    // 初始化应用程序
    bool Application::InitializeApplication(float width, float height, const std::string_view& title) {
        const char* glsl_version = "#version 330";
        if (!glfwInit()) return false;
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        main_scale_ = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
        // 这里我们只存 逻辑尺寸
        window_width_  = width;
        window_height_ = height;

        // GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        // const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

        // 创建窗口 —— 别忘了自定义 deleter
        window_ = std::unique_ptr<GLFWwindow, WindowDeleter>(
            glfwCreateWindow(window_width_, window_height_, title.data(), nullptr, nullptr), DeleteWindow
        );

        // window_ = std::unique_ptr<GLFWwindow, WindowDeleter>(
        //     glfwCreateWindow(mode->width, mode->height, title.data(), primaryMonitor, nullptr), DeleteWindow
        // );
        // window_width_  = mode->width / 1.5;
        // window_height_ = mode->height / 1.5;

        if (window_ == nullptr) {
            std::cerr << "Failed to create GLFW window." << '\n';
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window_.get());
        glfwSwapInterval(0);                           // 垂直同步 Vsync
        glfwSetWindowUserPointer(window_.get(), this); // 设置 user pointer

        // 注册回调
        glfwSetCursorPosCallback(window_.get(), CursorPosCallback);
        glfwSetFramebufferSizeCallback(window_.get(), FramebufferSizeCallback);

        // IMGUI 配置
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale_);
        style.FontScaleDpi = main_scale_;
        style.FontSizeBase = 15.0f;
        io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc");

        ImGui_ImplGlfw_InitForOpenGL(window_.get(), true);
        ImGui_ImplOpenGL3_Init(glsl_version); // 内部会进行 gladLoadGLLoader 的调用

        // 记得乘以缩放因子
        glViewport(0, 0, window_width_, window_height_);

        // 初始化 Renderer
        Renderer::Init();

        return true;
    }

    void Application::Run() {

        for (const auto& behavior : behaviors_) {
            behavior->Start();
        }

        auto lastTime = glfwGetTime();
        while (!glfwWindowShouldClose(window_.get())) {

#ifdef CPU_PROFILER
            auto t_start = std::chrono::high_resolution_clock::now();
#endif

            deltaTime_ = glfwGetTime() - lastTime;
            lastTime   = glfwGetTime();

            // 轮询和处理事件（输入、窗口大小调整等）
            glfwPollEvents();

#ifdef CPU_PROFILER
            auto t_poll = std::chrono::high_resolution_clock::now();
#endif

            // 最小化窗口时暂停渲染
            if (glfwGetWindowAttrib(window_.get(), GLFW_ICONIFIED) != 0) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            auto camera = Camera::Main();
            if (camera != nullptr) {
                Renderer::BeginScene(*camera);
            }

#ifdef CPU_PROFILER
            auto t_update_start = std::chrono::high_resolution_clock::now();
#endif

            // 更新游戏逻辑
            for (const auto& behavior : behaviors_) {
                behavior->Update(deltaTime_);
            }

#ifdef CPU_PROFILER
            auto t_update_end       = std::chrono::high_resolution_clock::now();
            auto t_render_cpu_start = std::chrono::high_resolution_clock::now();
            ImGui::Begin("Performance Profiler");
#endif

            Renderer::EndScene();


#ifdef CPU_PROFILER
            auto t_render_cpu_end = std::chrono::high_resolution_clock::now();

            // 计算各阶段耗时 (毫秒)
            std::chrono::duration<double, std::milli> d_poll   = t_poll - t_start;
            std::chrono::duration<double, std::milli> d_update = t_update_end - t_update_start;
            std::chrono::duration<double, std::milli> d_render = t_render_cpu_end - t_render_cpu_start;

            // 使用静态变量保存 SwapBuffers 的时间，因为当前帧的 Swap 还没发生，
            // 我们显示的是“上一帧”的 Swap 时间，或者放在 Swap 之后再计算也行，但那样 ImGui 就画不出来的。
            // 这里我们显示上一帧的 Swap 时间。
            static double last_swap_time = 0.0;
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Separator();
            ImGui::Text("Event Poll:   %.3f ms", d_poll.count());
            ImGui::Text("Game Update:  %.3f ms (Logic & Math)", d_update.count());
            ImGui::Text("Render CPU:   %.3f ms (Sort & Batching)", d_render.count());
            ImGui::Text("Swap Buffers: %.3f ms (GPU/VSync Wait)", last_swap_time);

            if (last_swap_time > 10.0) {
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "WAITING FOR GPU/VSYNC");
            }
            if (d_render.count() > 2.0) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "HEAVY BATCHING DETECTED");
            }
            ImGui::End();
#endif


            for (const auto& behavior : behaviors_) {
                behavior->OnDrawImGui();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef CPU_PROFILER
            auto t_swap_start = std::chrono::high_resolution_clock::now();
#endif

            glfwSwapBuffers(window_.get());


#ifdef CPU_PROFILER
            auto t_swap_end                                  = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> d_swap = t_swap_end - t_swap_start;
            last_swap_time                                   = d_swap.count();
#endif
        }
    }

    void Application::Close() {
        if (window_ != nullptr) {
            glfwSetWindowShouldClose(window_.get(), GLFW_TRUE);
        }
    }

    void Application::DeleteWindow(GLFWwindow* window) {
        if (window != nullptr) {
            glfwDestroyWindow(window);
        }
    }

    void Application::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));

        // 注意这里取出来的 behavior 是智能指针, 因此即使用 const 修饰, 也可以调用内部指针的非常函数
        for (const auto& behavior : app->behaviors_) {
            behavior->OnCursorPosChanged(xpos, ypos);
        }
    }

    void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
        auto app = static_cast<Application*>(glfwGetWindowUserPointer(window));
        for (const auto& behavior : app->behaviors_) {
            behavior->OnFramebufferResized(width, height);
        }
    }

} // namespace hamu
