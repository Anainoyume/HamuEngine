include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        main
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(TestMath
    test_math.cpp
)

target_include_directories(TestMath PRIVATE
    ${PROJECT_SOURCE_DIR}/src/core/math
)

target_link_libraries(TestMath PRIVATE
    glm
    GTest::gtest
    GTest::gtest_main
)

target_compile_options(TestMath PRIVATE
    -O3
    -march=native
)

# 把 GoogleTest 的 TEST() 自动注册成 CTest 的测试项，
# 让 CTest 和各种 IDE 能发现、运行和调试每一个单独的测试，
# 而不只是整个测试程序
include(GoogleTest)
gtest_discover_tests(TestMath)
