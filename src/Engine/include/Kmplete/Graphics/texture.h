#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"

#include <cstdint>

namespace Kmplete
{
    class Texture
    {
    public:
        KMP_API explicit Texture(const Path& filename, bool flipVertically = false);
        KMP_API explicit Texture(const char* filename, bool flipVertically = false);
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API void* GetHandle() const;

    protected:
        uint64_t _handle;
    };
    //--------------------------------------------------------------------------
}