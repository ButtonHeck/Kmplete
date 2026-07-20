#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"


namespace Kmplete
{
    //! Simple wrapper of a window cursor object
    //! @see Window
    class KMP_API WindowCursor
    {
    public:
        virtual ~WindowCursor() = default;

        KMP_NODISCARD virtual NonNull<void*> GetImplPointer() const noexcept = 0;
    };
    //--------------------------------------------------------------------------
}