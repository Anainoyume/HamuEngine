#pragma once

#ifdef HAMU_PLATFORM_WINDOWS
#ifdef HAMU_BUILD_DLL
#define HAMU_API __declspec(dllexport)
#else
#define HAMU_API __declspec(dllimport)
#endif
#else
#error HamuEngine only support Windows!
#endif