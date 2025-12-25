#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"


namespace Kmplete
{
    //! Base class for texture objects backed by graphics API
    class Texture
    {
    public:
        KMP_API Texture() noexcept;
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API Nullable<void*> GetHandle() const;

    protected:
        UInt64 _handle;
    };
    //--------------------------------------------------------------------------
}