#pragma once

#include "Kmplete/Base/platform.h"

#if defined (KMP_ENGINE_STATIC)
    #define KMP_API
    #define KMP_NO_EXPORT
#else
    #if defined (KMP_PLATFORM_WINDOWS)
        #if defined (KMP_EXPORTS)
            #define KMP_API __declspec(dllexport)
        #else
            #define KMP_API __declspec(dllimport)
        #endif
        #define KMP_NO_EXPORT 
    #else
        #define KMP_API __attribute__((visibility("default")))
        #define KMP_NO_EXPORT __attribute__((visibility("hidden")))
    #endif    
#endif

#if defined (KMP_PLATFORM_WINDOWS)
    #define KMP_DEPRECATED __declspec(deprecated)
#else
    #define KMP_DEPRECATED __attribute__((deprecated))
#endif

#define KMP_DEPRECATED_EXPORT KMP_API KMP_DEPRECATED
#define KMP_DEPRECATED_NO_EXPORT KMP_NO_EXPORT KMP_DEPRECATED

#define KMP_NORETURN [[noreturn]]
#define KMP_NODISCARD [[nodiscard]]
#define KMP_MB_UNUSED [[maybe_unused]]

#define KMP_DISABLE_COPY(ClassName)\
    ClassName(const ClassName&) = delete;\
    ClassName& operator=(const ClassName&) = delete;\

#define KMP_DISABLE_MOVE(ClassName)\
    ClassName(ClassName&&) = delete;\
    ClassName& operator=(ClassName&&) = delete;

#define KMP_DISABLE_COPY_MOVE(ClassName)\
	KMP_DISABLE_COPY(ClassName)\
	KMP_DISABLE_MOVE(ClassName)
