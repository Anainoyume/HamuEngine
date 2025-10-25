cmake_minimum_required(VERSION 3.20)

project(HamuEngine VERSION 1.0 LANGUAGES CXX)

#========= Settings ============================================================================================

    # 导出 compile commands, clangd 需要参考这个给运行语言服务器
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
#===============================================================================================================





#========= HamuEngine dll ======================================================================================
    
    # 搜索 src 文件
    file(GLOB_RECURSE HAMUENGINE_SOURCES "HamuEngine/src/*.cpp" "HamuEngine/src/*.h")
    add_library(EngineCore SHARED ${HAMUENGINE_SOURCES})

    target_include_directories(EngineCore PRIVATE
        "${PROJECT_SOURCE_DIR}/HamuEngine/src"
        "${PROJECT_SOURCE_DIR}/HamuEngine/include"
    )

    target_link_directories(EngineCore PRIVATE 
        "${PROJECT_SOURCE_DIR}/HamuEngine/lib"
    )

    target_link_libraries(EngineCore PRIVATE 
        winpthread
        opengl32
        libglfw3dll.a       # glfw 动态导入库
        libspdlog.dll.a
        $<$<BOOL:${MINGW}>:ws2_32>
    )

    target_compile_definitions(EngineCore PRIVATE
        # ...
    )
#===============================================================================================================





#========= HamuEngine dll ======================================================================================
    
    # 搜索 src 文件
    file(GLOB_RECURSE SANDBOX_SOURCES "SandBox/src/*.cpp" "SandBox/src/*.h")
    add_executable(SandBox ${SANDBOX_SOURCES})

    target_include_directories(SandBox PRIVATE
        "${PROJECT_SOURCE_DIR}/HamuEngine/src"
    )

    # 链接我们的引擎
    target_link_directories(SandBox PRIVATE 
        EngineCore
    )

    target_link_libraries(SandBox PRIVATE 
        
    )

    target_compile_definitions(SandBox PRIVATE
        # ...
    )
#===============================================================================================================





#========= Configuration =======================================================================================
    
    # 配置不同的构建类型
    # foreach(CONFIGURATION Debug Release Dist)
    #     string(TOUPPER ${CONFIGURATION} CONFIG)
    #     if (CMAKE_BUILD_TYPE STREQUAL ${CONFIG})
    #         target_compile_definitions(Jigsaw PRIVATE JS_DEBUG)
    #         target_compile_definitions(Jigsaw PRIVATE JS_ENABLE_ASSERTS)

    #         target_compile_definitions(Sandbox PRIVATE JS_DEBUG)
    #     endif()

    #     if (CMAKE_BUILD_TYPE STREQUAL Release)
    #         target_compile_definitions(Jigsaw PRIVATE JS_RELRASE)
    #         target_compile_definitions(Sandbox PRIVATE JS_RELRASE)
    #         target_compile_options(Jigsaw PRIVATE -O2)
    #         target_compile_options(Sandbox PRIVATE -O2)
    #     endif()

    #     if (CMAKE_BUILD_TYPE STREQUAL Dist)
    #         target_compile_definitions(Jigsaw PRIVATE JS_DIST)
    #         target_compile_definitions(Sandbox PRIVATE JS_DIST)
    #         target_compile_options(Jigsaw PRIVATE -O2)
    #         target_compile_options(Sandbox PRIVATE -O2)
    #     endif()
    # endforeach()

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(PLATFORM "x64")
    else()
        set(PLATFORM "x86")
    endif()

    # 设置库和可执行文件的输出目录
    set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
    # 设置静态库, 动态库, 执行文件的输出目录
    set_target_properties(EngineCore PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/HamuEngine/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${PLATFORM}"
            LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/HamuEngine/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${PLATFORM}"
            RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/HamuEngine/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${PLATFORM}"
    )

    set_target_properties(SandBox PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/SandBox/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${PLATFORM}"
            LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/SandBox/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${PLATFORM}"
            RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/SandBox/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${PLATFORM}"
    )
    

    # 设置输出拷贝命令
    add_custom_command(TARGET EngineCore SandBox POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:EngineCore> $<TARGET_FILE_DIR:SandBox>
    )
#===============================================================================================================
