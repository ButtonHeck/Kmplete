#pragma once

#if defined (_MSC_VER)
    #define KMP_COMPILER_MSVC
#elif defined (__MINGW32__) || defined (__MINGW64__)
    #define KMP_COMPILER_MINGW
#elif defined (__GNUC__)
    #define KMP_COMPILER_GCC
#elif defined(__clang__)
    #define KMP_COMPILER_CLANG
#else
    #define KMP_COMPILER_OTHER 
#endif

#if defined (_WIN32) || defined (_WIN64)
    #define KMP_PLATFORM_WINDOWS
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    #define KMP_PLATFORM_LINUX
#elif define (__APPLE__)
    #define KMP_PLATFORM_MACOSX
#else
    #define KMP_PLATFORM_OTHER
#endif
