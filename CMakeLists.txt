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
    "src/runtime/render/Shader.cpp"
    "src/runtime/render/Texture.cpp"
    "src/runtime/render/Material.cpp"
    "src/runtime/render/Mesh.cpp"
    "src/runtime/render/Renderer.cpp"
    "src/runtime/render/MeshRenderer.cpp"
    
    "src/runtime/render/buffer/VertexBuffer.cpp"
    "src/runtime/render/buffer/IndexBuffer.cpp"
    "src/runtime/render/buffer/UniformBuffer.cpp"
    "src/runtime/render/buffer/VertexArray.cpp"
    "src/runtime/render/buffer/VertexLayout.cpp"

    "src/runtime/transform/Transform.cpp"
    "src/runtime/camera/Camera.cpp"
    "src/runtime/application/Application.cpp"

    "src/utils/tiny_json/JsonNode.cpp"
    "src/utils/tiny_json/JsonLexer.cpp"
    "src/utils/tiny_json/JsonParser.cpp"
    "src/utils/tiny_json/JsonDomBuilder.cpp"
    "src/utils/tiny_json/JsonUtils.cpp"
)

add_library(HamuEngine STATIC ${ENGINE_SRC})
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
    # HAMU_ENGINE_DLL_EXPORT
    HAMU_ENGINE_BUILD_STATIC
)



# ---------------------------------- SandBox -----------------------------------
set(SANDBOX_SRC
    "sandbox/main.cpp"
)

add_executable(SandBox ${SANDBOX_SRC})
target_include_directories(SandBox PRIVATE 
    "sandbox"
)
target_link_libraries(SandBox PRIVATE 
    ${COMMON_LIB_NAME}
    HamuEngine    

    glad
    glfw
    glm
    imgui
    stb_image
)
target_compile_definitions(SandBox PRIVATE
    HAMU_ENGINE_BUILD_STATIC # 临时改成静态链接库
)
