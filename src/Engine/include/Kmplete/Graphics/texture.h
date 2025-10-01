#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API void* GetHandle() const;

    protected:
        UInt64 _handle;
    };
    //--------------------------------------------------------------------------
}