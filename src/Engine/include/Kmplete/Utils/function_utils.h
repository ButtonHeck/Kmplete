#pragma once

#define KMP_BIND(func) [this](auto&& ... args) { return this->func(std::forward<decltype(args)>(args)...); }

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