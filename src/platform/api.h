#pragma once

#if defined(_WIN32)
    #ifdef HAMU_ENGINE_DLL_EXPORT
        #define HAMU_API __declspec(dllexport)
    #else
        #define HAMU_API __declspec(dllimport)
    #endif
#else
    #define HAMU_API
#endif
