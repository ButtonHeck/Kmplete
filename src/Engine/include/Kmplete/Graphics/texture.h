#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <cstdint>

namespace Kmplete
{
    class Texture
    {
    public:
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API void* GetHandle() const;

    protected:
        uint64_t _handle;
    };
    //--------------------------------------------------------------------------
}