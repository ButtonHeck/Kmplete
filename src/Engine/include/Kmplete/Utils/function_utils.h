#pragma once

#include "Kmplete/Core/platform.h"
#include "Kmplete/Core/kmplete_api.h"

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
        KMP_CONSTEXPR size_t GetArgsCount(TArgs&&...)
        {
            return sizeof...(TArgs);
        }
        //--------------------------------------------------------------------------
    }
}