cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 递归搜索 src 文件夹内的 .c/.cpp 文件
file(GLOB SRC
    "src/*.cpp"
    "src/lib/*.cpp"
)

# Project: app
project(app)

add_executable(app ${SRC})

target_include_directories(app PRIVATE
    "include"
)

target_link_directories(app PRIVATE
    "lib"
)

target_link_libraries(app PRIVATE 
    winpthread
    opengl32
    libglfw3dll.a
)

target_compile_options(app PRIVATE
    -fdiagnostics-show-template-tree
    -ferror-limit=1
)

