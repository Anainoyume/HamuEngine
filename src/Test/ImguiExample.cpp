#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h" // 绘制将使用系统OpenGL标头

#include <iostream>
#include <stdio.h>

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

// 主要代码
int main(int, char**) {

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) return 1;

    // 确定 GL+GLSL 版本
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //仅限 3.2+
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            //仅限 3.0+

    // 创建带有图形上下文的窗口
    // glfwGetPrimaryMonitor() 用于获取主监视器以查询内容缩放
    float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // 仅适用于 GLFW 3.3+

    GLFWwindow* window = glfwCreateWindow(
        (int)(1280 * main_scale), (int)(800 * main_scale), "Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr
    );
    if (window == nullptr) return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // 启用垂直同步 Vsync


    // ---------------------------- 设置 Dear ImGui 上下文 ------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘控制
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // 启用游戏手柄控制

    // 设置 Dear ImGui 风格
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // 设置缩放比例
    ImGuiStyle& style = ImGui::GetStyle();

    // 烘焙固定风格规模。 （在我们找到动态样式缩放的解决方案之前，更改此设置需要重置 Style + 再次调用此设置）
    style.ScaleAllSizes(main_scale);

    // 设置初始字体比例。 （使用 io.ConfigDpiScaleFonts=true 使得这没有必要。出于文档目的，我们将两者都留在这里）
    style.FontScaleDpi = main_scale;


    // 加载字体
    // -如果没有加载字体，亲爱的imgui将使用默认字体。您还可以加载多种字体并使用 ImGui::PushFont()/PopFont() 来选择它们。
    // -AddFontFromFileTTF() 将返回 ImFont*，因此如果您需要在多个字体中选择字体，您可以存储它。
    // -如果文件无法加载，该函数将返回 nullptr。请在您的应用程序中处理这些错误（例如使用断言，或显示错误并退出）。
    // -在 imconfig 文件中使用“#define IMGUI_ENABLE_FREETYPE”以使用 Freetype 实现更高质量的字体渲染。
    // -阅读“docs/FONTS.md”以获取更多说明和详细信息。
    // -请记住，在 C/C++ 中，如果要在字符串文字中包含反斜杠 \，则需要编写双反斜杠 \\！
    // -我们的 Emscripten 构建过程允许在运行时从“fonts/”文件夹访问嵌入字体。有关详细信息，请参阅 Makefile.emscripten。
    // style.FontSizeBase = 20.0f;
    // io.Fonts->AddFontDefault();
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
    // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
    // ImFont*font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
    // IM_ASSERT(font != nullptr);
    
    style.FontSizeBase = 15.0f;
    io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msyh.ttc");
    // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\consola.ttf");

    // -----------------------------------------------------------------------------




    // 设置平台/渲染器后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    



    // 我们的状态
    bool show_demo_window    = true;
    bool show_another_window = false;
    ImVec4 clear_color       = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        // 轮询和处理事件（输入、窗口大小调整等）
        // 您可以读取 io.WantCaptureMouse、io.WantCaptureKeyboard 标志来判断亲爱的 imgui 是否想要使用您的输入。
        // -当 io.WantCaptureMouse 为 true 时，不要将鼠标输入数据分派到主应用程序，或清除/覆盖鼠标数据的副本。
        // -当 io.WantCaptureKeyboard 为 true 时，不要将键盘输入数据分派到主应用程序，或清除/覆盖键盘数据的副本。
        // 一般来说，您可能总是将所有输入传递给亲爱的 imgui，并根据这两个标志将它们隐藏在您的应用程序中。
        glfwPollEvents();
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // 启动亲爱的 ImGui 框架
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();




        // 1. 显示大的演示窗口（大部分示例代码在 ImGui::ShowDemoWindow() 中！您可以浏览其代码来了解更多关于 Dear ImGui！）。
        if (show_demo_window) ImGui::ShowDemoWindow(&show_demo_window);

        // 2. 显示一个我们自己创建的简单窗口。我们使用开始/结束对来创建一个命名窗口。
        {
            static float f     = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                     // 创建一个名为“Hello, world!”的窗口并附加到其中。

            ImGui::Text("This is some useful text.");          // 显示一些文本（您也可以使用格式字符串）
            ImGui::Checkbox("Demo Window", &show_demo_window); // 编辑存储窗口打开/关闭状态的布尔值
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // 使用 0.0f 到 1.0f 之间的滑块编辑 1 个浮点数
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // 编辑代表颜色的 3 个浮点数

            if (ImGui::Button("Button")) // 单击按钮时返回 true（大多数小部件在编辑/激活时返回 true）
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // 3. 显示另一个简单窗口。
        if (show_another_window) {
            ImGui::Begin(
                "Another Window", &show_another_window
            ); // 将指针传递给我们的 bool 变量（窗口将有一个关闭按钮，单击时将清除 bool）
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
            ImGui::End();
        }




        // 渲染
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(
            clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w
        );
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        glfwSwapBuffers(window);
    }

    // 清理
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
