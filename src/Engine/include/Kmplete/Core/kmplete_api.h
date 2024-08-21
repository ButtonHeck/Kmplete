#pragma once

#include "Kmplete/Core/platform.h"

#ifdef Kmplete_Engine_STATIC
    #define KMP_API
    #define KMP_NO_EXPORT
#else
    #ifdef KMP_PLATFORM_WINDOWS
        #ifdef Kmplete_Engine_EXPORTS
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

#ifdef KMP_PLATFORM_WINDOWS
    #define KMP_DEPRECATED __declspec(deprecated)
#else
    #define KMP_DEPRECATED
#endif

#define KMP_DEPRECATED_EXPORT KMP_API KMP_DEPRECATED
#define KMP_DEPRECATED_NO_EXPORT KMP_NO_EXPORT KMP_DEPRECATED

#define KMP_CONSTEXPR constexpr
#define KMP_NOEXCEPT noexcept
#define KMP_NORETURN [[noreturn]]
#define KMP_NODISCARD [[nodiscard]]
#define KMP_MB_UNUSED [[maybe_unused]]

#define KMP_DISABLE_COPY_MOVE(ClassName)\
	ClassName(const ClassName&) = delete;\
	ClassName(ClassName&&) = delete;\
	const ClassName& operator=(const ClassName&) = delete;\
	void operator=(ClassName&&) = delete;
