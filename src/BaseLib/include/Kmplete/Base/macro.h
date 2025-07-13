#pragma once

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
