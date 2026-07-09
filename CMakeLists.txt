cmake_minimum_required(VERSION 3.20)

project(app)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- glad 单独处理 ----
set(GLAD_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/glad.c")
set_source_files_properties(${GLAD_SRC} PROPERTIES LANGUAGE C)

# 自己的代码：显式列
set(ENGINE_SRC
    "src/core/render/Shader.cpp"
    "src/core/render/Texture.cpp"
    "src/core/render/Material.cpp"
    "src/core/render/Mesh.cpp"
    "src/core/render/Renderer.cpp"
    "src/core/render/MeshRenderer.cpp"
    
    "src/core/render/buffer/VertexBuffer.cpp"
    "src/core/render/buffer/IndexBuffer.cpp"
    "src/core/render/buffer/UniformBuffer.cpp"
    "src/core/render/buffer/VertexArray.cpp"
    "src/core/render/buffer/VertexLayout.cpp"

    "src/core/transform/Transform.cpp"
    "src/core/camera/Camera.cpp"
    "src/core/application/Application.cpp"

    "src/utils/tiny_json/JsonNode.cpp"
    "src/utils/tiny_json/JsonLexer.cpp"
    "src/utils/tiny_json/JsonParser.cpp"
    "src/utils/tiny_json/JsonDomBuilder.cpp"
    "src/utils/tiny_json/JsonUtils.cpp"
)

# 第三方代码：imgui
set(IMGUI_SRC
    "include/imgui/imgui.cpp"
    "include/imgui/imgui_draw.cpp"
    "include/imgui/imgui_widgets.cpp"
    "include/imgui/imgui_tables.cpp"
    "include/imgui/imgui_demo.cpp"
    "include/imgui/backends/imgui_impl_glfw.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
    "include/imgui/backends/imgui_impl_glfw.cpp"
)

# ------------------------------------ 合并 --------------------------------------
add_executable(lab
    "src/Lesson4.cpp"

    ${ENGINE_SRC}
    ${IMGUI_SRC}
    ${GLAD_SRC}
)

target_include_directories(lab PRIVATE 
    "include" 
    "include/imgui"
    "include/stb_image"
    "src/core"
    "src/utils"
)
target_link_directories(lab PRIVATE "lib")
target_link_libraries(lab PRIVATE 
    winpthread
    opengl32
    libglfw3.a
)

# 添加宏定义，使用自定义的 OpenGL 加载器
target_compile_definitions(lab PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
    # CPU_PROFILER
)



# ----------------------------- App Configuration ------------------------------
add_executable(app 
    "src/Test/ImguiExample.cpp"

    "include/imgui/imgui.cpp"
    "include/imgui/imgui_draw.cpp"
    "include/imgui/imgui_widgets.cpp"
    "include/imgui/imgui_tables.cpp"
    "include/imgui/imgui_demo.cpp"
    "include/imgui/backends/imgui_impl_glfw.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
    "include/imgui/backends/imgui_impl_opengl3.cpp"
    "include/imgui/backends/imgui_impl_glfw.cpp"

    ${GLAD_SRC}
)

target_include_directories(app PRIVATE
    "include"
    "include/imgui"
)

target_link_directories(app PRIVATE
    "lib"
)

target_link_libraries(app PRIVATE 
    winpthread
    opengl32
    libglfw3.a
)

target_compile_options(app PRIVATE
    -fdiagnostics-show-template-tree
    -ferror-limit=1
)

# 添加宏定义，使用自定义的 OpenGL 加载器
target_compile_definitions(app PRIVATE
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)


# ---------------------------------- SandBox -----------------------------------
add_executable(sandbox
    "src/Test/SandBox.cpp"
    "src/utils/tiny_json/JsonNode.cpp"
    "src/utils/tiny_json/JsonLexer.cpp"
    "src/utils/tiny_json/JsonParser.cpp"
    "src/utils/tiny_json/JsonDomBuilder.cpp"
    "src/utils/tiny_json/JsonUtils.cpp"
    "src/utils/tiny_json/example/test01.cpp"

    "src/core/transform/Transform.cpp"
)

target_include_directories(sandbox PRIVATE 
    "include" 
    "src/core"
    "src/Test"
    "src/Test/include"
    "src/utils"
)


target_link_directories(sandbox PRIVATE 
    "lib"
)

target_link_libraries(sandbox PRIVATE 
    winpthread
    opengl32
    libglfw3.a
)


