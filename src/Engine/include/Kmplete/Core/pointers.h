#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <memory>

namespace Kmplete
{
    template<typename T>
    using Ptr = std::shared_ptr<T>;

    template<typename T, typename... Args>
    KMP_NODISCARD KMP_CONSTEXPR Ptr<T> CreatePtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    //--------------------------------------------------------------------------

    template<typename T>
    using UPtr = std::unique_ptr<T>;

    template<typename T, typename... Args>
    KMP_NODISCARD KMP_CONSTEXPR UPtr<T> CreateUPtr(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
    //--------------------------------------------------------------------------
}
