#pragma once

#if defined (_MSC_VER)
    #define KMPLETE_COMPILER_MSVC
#elif defined (__MINGW32__) || defined (__MINGW64__)
    #define KMPLETE_COMPILER_MINGW
#elif defined (__GNUC__)
    #define KMPLETE_COMPILER_GCC
#elif defined(__clang__)
    #define KMPLETE_COMPILER_CLANG
#endif

#if defined (_WIN32) || defined (_WIN64)
    #define KMPLETE_PLATFORM_WINDOWS
    #define NOGDI
    #if defined (KMPLETE_COMPILER_MSVC)
        #define NOMINMAX
    #endif
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    #define KMPLETE_PLATFORM_LINUX
#elif define (__APPLE__)
    #define KMPLETE_PLATFORM_MACOSX
#endif
