#pragma once

#if defined(_WIN32) && !defined(HAMU_ENGINE_BUILD_STATIC)
    #ifdef HAMU_ENGINE_DLL_EXPORT
        #define HAMU_API __declspec(dllexport)
    #else
        #define HAMU_API __declspec(dllimport)
    #endif
#else
    #define HAMU_API
#endif
