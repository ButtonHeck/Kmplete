#pragma once

#include "kmplete_export.h"

#define KMPLETE_BIND(func) [this](auto&& ... args) { return this->func(std::forward<decltype(args)>(args)...); }

namespace Kmplete
{
    namespace Utils
    {
        template<typename ...TArgs>
        KMPLETE_API constexpr size_t GetArgsCount(TArgs&&...)
        {
            return sizeof...(TArgs);
        }
        //--------------------------------------------------------------------------
    }
}