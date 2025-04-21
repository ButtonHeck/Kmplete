#pragma once

#include "Kmplete/Core/kmplete_api.h"

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