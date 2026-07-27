set(HAMU_ENGINE_SRC
    runtime/render/Shader.cpp
    runtime/render/Texture.cpp
    runtime/render/Material.cpp
    runtime/render/Mesh.cpp
    runtime/render/Renderer.cpp
    runtime/render/MeshRenderer.cpp

    runtime/render/buffer/VertexBuffer.cpp
    runtime/render/buffer/IndexBuffer.cpp
    runtime/render/buffer/UniformBuffer.cpp
    runtime/render/buffer/VertexArray.cpp
    runtime/render/buffer/VertexLayout.cpp

    runtime/transform/Transform.cpp
    runtime/camera/Camera.cpp
    runtime/application/Application.cpp

    utils/tiny_json/JsonNode.cpp
    utils/tiny_json/JsonLexer.cpp
    utils/tiny_json/JsonParser.cpp
    utils/tiny_json/JsonDOMBuilder.cpp
    utils/tiny_json/JsonUtils.cpp
)

add_library(HamuEngine STATIC ${HAMU_ENGINE_SRC})

# 公开一下头文件路径
target_include_directories(HamuEngine PUBLIC
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(HamuEngine PRIVATE
    ${HAMU_ENGINE_COMMON_LIBS}
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
