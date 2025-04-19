#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Graphics/graphics_base.h"

#include <cstdint>

namespace Kmplete
{
    class Texture
    {
    public:
        KMP_API Texture(const Path& filename, ImageChannels desiredChannels, bool flipVertically = false);
        KMP_API Texture(const char* filename, ImageChannels desiredChannels, bool flipVertically = false);
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API void* GetHandle() const;

    protected:
        uint64_t _handle;
    };
    //--------------------------------------------------------------------------
}