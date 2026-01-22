#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/platform.h"

#if !defined (KMP_COMPILER_MSVC)
    #include <cstddef>
#endif


//! Shortcut for binding a class member function, e.g.:
//! mainWindow.SetEventCallback(KMP_BIND(WindowApplication::OnEvent));
#define KMP_BIND(func) [this](auto&& ... args) { return this->func(std::forward<decltype(args)>(args)...); }

//! Shortcut for binding a free function
#define KMP_BIND_FREE_FN(func) [](auto&& ... args) { return func(std::forward<decltype(args)>(args)...); }


namespace Kmplete
{
    //! Other helper functions
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
