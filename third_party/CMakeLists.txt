set(THIRD_PARTY ${CMAKE_CURRENT_SOURCE_DIR})
set(GLAD_ROOT ${THIRD_PARTY}/glad)
set(GLFW_ROOT ${THIRD_PARTY}/glfw)
set(GLM_ROOT ${THIRD_PARTY}/glm)
set(IMGUI_ROOT ${THIRD_PARTY}/imgui)
set(STB_IMAGE_ROOT ${THIRD_PARTY}/stb_image)


# glad 库
add_library(glad STATIC ${GLAD_ROOT}/glad.cpp)
target_include_directories(glad PUBLIC ${GLAD_ROOT})


# glfw 库
add_library(glfw INTERFACE)
target_link_libraries(glfw INTERFACE ${GLFW_ROOT}/lib/libglfw3.a)
target_include_directories(glfw INTERFACE ${GLFW_ROOT})


# glm 库
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${GLM_ROOT})


# imgui 库
add_library(imgui STATIC
    ${IMGUI_ROOT}/imgui_demo.cpp
    ${IMGUI_ROOT}/imgui_draw.cpp
    ${IMGUI_ROOT}/imgui_tables.cpp
    ${IMGUI_ROOT}/imgui_widgets.cpp
    ${IMGUI_ROOT}/imgui.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp
    ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PRIVATE ${GLFW_ROOT}) # 用到了 glfw
target_include_directories(imgui PUBLIC ${IMGUI_ROOT})
target_compile_definitions(imgui INTERFACE
    # 我们改了 imgui 源码, 修改了自定义的 loader, 这里加上宏
    IMGUI_IMPL_OPENGL_LOADER_CUSTOM
)


# stb_image 库
add_library(stb_image STATIC ${STB_IMAGE_ROOT}/stb_image.cpp)
target_include_directories(stb_image PUBLIC ${STB_IMAGE_ROOT})