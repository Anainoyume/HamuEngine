cmake_minimum_required(VERSION 3.20)

# 导出 compile commands, clangd 需要参考这个给运行语言服务器
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 递归搜索 src 文件夹内的 .c/.cpp 文件
file(GLOB SRC
    "src/*.cpp"
)

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/include/imgui")
set(IMGUI_SRC
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
    ${IMGUI_DIR}/imgui_demo.cpp   # 如果你要用 Demo 窗口，就加上
)

# Project: app
project(app)

add_executable(app ${SRC} ${IMGUI_SRC})

target_include_directories(app PRIVATE
    "include"
    "include/imgui"
    "include/imgui/backends"
)

target_link_directories(app PRIVATE
    "lib"
)

target_link_libraries(app PRIVATE 
    winpthread
    opengl32
    libglfw3dll.a
    libspdlog.dll.a
    ws2_32
)

# -------- 编译选项按构建类型区分 --------
# Debug: 无优化，调试信息全开
target_compile_options(app PRIVATE
    $<$<CONFIG:Debug>:
        -O0
        -g
        -fno-inline
        -fno-builtin
        -fdiagnostics-show-template-tree
        -ferror-limit=1
        -fno-elide-constructors
    >
)

# Release: 优化全开
target_compile_options(app PRIVATE
    $<$<CONFIG:Release>:
        -O2
        -DNDEBUG
    >
)
