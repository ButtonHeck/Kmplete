#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Assets/asset.h"


namespace Kmplete
{
    class Texture : public Assets::Asset
    {
    public:
        KMP_NODISCARD KMP_API static unsigned int GetMipLevelsCount(int width, int height) noexcept;

    public:
        KMP_API explicit Texture(Utils::StringID sid) noexcept;
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API Nullable<void*> GetHandle() const;

    protected:
        UInt64 _handle;
    };
    //--------------------------------------------------------------------------
}