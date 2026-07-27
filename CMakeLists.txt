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

option(HAMU_ENGINE_BUILD_SANDBOX "Build sandbox executables" ON)
option(HAMU_ENGINE_BUILD_BENCHMARKS "Build benchmark executables" ON)
option(HAMU_ENGINE_BUILD_TESTS "Build test executables" ON)

# 常用的库
set(HAMU_ENGINE_COMMON_LIBS
    winpthread
    opengl32
)

# 导入第三方库子构建目录
add_subdirectory(third_party)

add_subdirectory(src)

if(HAMU_ENGINE_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()

if(HAMU_ENGINE_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

if(HAMU_ENGINE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()
