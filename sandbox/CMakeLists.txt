add_executable(SandBox main.cpp)

target_include_directories(SandBox PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SandBox PRIVATE
    ${HAMU_ENGINE_COMMON_LIBS}
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



# ---------------------------------- SandBox-Test -----------------------------------
add_executable(SandBoxTest test.cpp)

target_include_directories(SandBoxTest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src/core/math
)

target_link_libraries(SandBoxTest PRIVATE
    glm
)

target_compile_options(SandBoxTest PRIVATE
    -O3
    -march=native
)
