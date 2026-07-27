include(FetchContent)

# benchmark 库我们直接远程引入
# 声明要获取的 Google Benchmark 库
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG        main    # 推荐使用稳定的发布版本
    GIT_SHALLOW    TRUE    # 只克隆最新提交，加快下载速度
)

# 在获取前，关闭 Benchmark 自身的测试，避免它们被构建
set(BENCHMARK_ENABLE_TESTING OFF)

# 使内容可用，CMake 会自动下载并编译
FetchContent_MakeAvailable(benchmark)

add_executable(Bench_Math bench_math.cpp)

target_link_libraries(Bench_Math PRIVATE
    benchmark::benchmark
    glm
)

target_include_directories(Bench_Math PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/math
)

# 优化拉满, 设置格式化输出
target_compile_options(Bench_Math PRIVATE
    -O3
    -march=native
)



add_executable(Bench_Math_Batch bench_math_batch.cpp)

target_link_libraries(Bench_Math_Batch PRIVATE
    benchmark::benchmark
    glm
)

target_include_directories(Bench_Math_Batch PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/math
)

target_compile_options(Bench_Math_Batch PRIVATE
    -O3
    -march=native
)

target_compile_definitions(Bench_Math_Batch PRIVATE GLM_BATCH)
