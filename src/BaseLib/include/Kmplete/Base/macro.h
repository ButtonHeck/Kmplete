#pragma once

#include "Kmplete/Base/platform.h"


#define KMP_M_CONCAT_(x, y) x ## y
#define KMP_M_CONCAT(x, y) KMP_M_CONCAT_(x, y)
#define KMP_M_STRINGIFY_(x) #x
#define KMP_M_STRINGIFY(x) KMP_M_STRINGIFY_(x)

#define KMP_M_NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, TOTAL, ...) TOTAL
#define KMP_M_NUM_ARGS(...) KMP_M_NUM_ARGS_(__VA_ARGS__, 12_, 11_, 10_, 9_, 8_, 7_, 6_, 5_, 4_, 3_, 2_, 1_)
#define KMP_M_DISPATCH_VA(macro, ...) KMP_M_CONCAT(macro, KMP_M_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
    #define KMP_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
    #define KMP_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
    #define KMP_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
    #define KMP_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
    #define KMP_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
    #define KMP_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
    #define KMP_FUNC_SIG __func__
#else
    #error "KMP_FUNC_SIG unknown!"
#endif

#if defined (KMP_COMPILER_MSVC)
    #define KMP_COMPILER_DIAGNOSTIC_PUSH
    #define KMP_COMPILER_DIAGNOSTIC_IGNORE(value)
    #define KMP_COMPILER_DIAGNOSTIC_POP
#elif defined (KMP_COMPILER_MINGW) || defined (KMP_COMPILER_GCC)
    #define KMP_COMPILER_DIAGNOSTIC_PUSH            _Pragma("GCC diagnostic push")
    #define KMP_COMPILER_DIAGNOSTIC_IGNORE(value)   _Pragma(KMP_M_STRINGIFY(GCC diagnostic ignored value))
    #define KMP_COMPILER_DIAGNOSTIC_POP             _Pragma("GCC diagnostic pop")
#elif defined (KMP_COMPILER_CLANG)
    #define KMP_COMPILER_DIAGNOSTIC_PUSH            _Pragma("clang diagnostic push")
    #define KMP_COMPILER_DIAGNOSTIC_IGNORE(value)   _Pragma(KMP_M_STRINGIFY(clang diagnostic ignored value))
    #define KMP_COMPILER_DIAGNOSTIC_POP             _Pragma("clang diagnostic pop")
#endif

#if defined (KMP_COMPILER_MSVC)
    #define KMP_BEGIN_PACKED_STRUCT(name)   _Pragma("pack(push, 1)") struct name
    #define KMP_END_PACKED_STRUCT           _Pragma("pack(pop)")
#elif defined (KMP_COMPILER_MINGW) || defined (KMP_COMPILER_GCC) || defined (KMP_COMPILER_CLANG)
    #define KMP_BEGIN_PACKED_STRUCT(name)   struct __attribute__((packed)) name
    #define KMP_END_PACKED_STRUCT
#endif
