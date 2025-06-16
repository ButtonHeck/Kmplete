#pragma once

#include "Kmplete/Base/platform.h"

#if defined (KMP_BUILD_STATIC)
    #define KMP_LOG_API
    #define KMP_LOG_NO_EXPORT
#else
    #if defined (KMP_PLATFORM_WINDOWS)
        #if defined (KMP_LOG_EXPORTS)
            #define KMP_LOG_API __declspec(dllexport)
        #else
            #define KMP_LOG_API __declspec(dllimport)
        #endif
        #define KMP_LOG_NO_EXPORT 
    #else
        #define KMP_LOG_API __attribute__((visibility("default")))
        #define KMP_LOG_NO_EXPORT __attribute__((visibility("hidden")))
    #endif    
#endif