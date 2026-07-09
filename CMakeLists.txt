cmake_minimum_required(VERSION 3.20)

project(HamuEngineProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 动态库输出目录 (.dll / .so)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 静态库输出目录 (.lib / .a)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 导入第三方库子构建目录
add_subdirectory(third_party)

# 常用的库
set(COMMON_LIB_NAME
    winpthread
    opengl32
)

# ----------------------------- App Configuration ------------------------------
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

add_library(HamuEngine SHARED ${ENGINE_SRC})
# 公开一下头文件路径
target_include_directories(HamuEngine PUBLIC 
    "src"
)
target_link_libraries(HamuEngine PRIVATE 
    ${COMMON_LIB_NAME}
    glad
    glfw
    glm
    imgui
    stb_image
)
target_compile_options(HamuEngine PRIVATE
    -fdiagnostics-show-template-tree
    -ferror-limit=1
)
target_compile_definitions(HamuEngine PRIVATE
    HAMU_ENGINE_DLL_EXPORT
)



# ---------------------------------- SandBox -----------------------------------
set(SANDBOX_SRC
    "sandbox/main.cpp"
)

add_executable(SandBox ${SANDBOX_SRC})
target_include_directories(SandBox PRIVATE "sandbox")
target_link_libraries(SandBox PRIVATE 
    ${COMMON_LIB_NAME}
    HamuEngine    
)

