#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"

namespace Kmplete
{
    class WindowCursor
    {
    public:
        virtual ~WindowCursor() = default;
        KMP_NODISCARD KMP_API virtual NonNull<void*> GetImplPointer() const noexcept = 0;
    };
    //--------------------------------------------------------------------------
}