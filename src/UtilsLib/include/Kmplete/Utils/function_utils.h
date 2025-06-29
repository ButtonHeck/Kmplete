#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"

#if !defined (KMP_COMPILER_MSVC)
    #include <cstddef>
#endif

#define KMP_BIND(func) [this](auto&& ... args) { return this->func(std::forward<decltype(args)>(args)...); }
#define KMP_BIND_FREE_FN(func) [](auto&& ... args) { return func(std::forward<decltype(args)>(args)...); }

namespace Kmplete
{
    namespace Utils
    {
        template<typename ...TArgs>
        KMP_NODISCARD constexpr size_t GetArgsCount(TArgs&&...)
        {
            return sizeof...(TArgs);
        }
        //--------------------------------------------------------------------------
    }
}

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