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
        constexpr size_t GetArgsCount(TArgs&&...)
        {
            return sizeof...(TArgs);
        }
        //--------------------------------------------------------------------------
    }
}