#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Graphics/texture_filtering.h"


namespace Kmplete
{
    //! Base class for texture objects backed by graphics API
    class Texture
    {
        KMP_DISABLE_COPY_MOVE(Texture)

    public:
        KMP_API Texture() noexcept;
        virtual ~Texture() = default;

        KMP_NODISCARD KMP_API Nullable<void*> GetHandle() const;

        KMP_NODISCARD KMP_API const TextureFiltering& GetFiltering() const;
        KMP_API void SetFiltering(const TextureFiltering& filtering);

    protected:
        virtual void _SetFilteringImpl() = 0;

    protected:
        UInt64 _handle;
        TextureFiltering _filtering;
    };
    //--------------------------------------------------------------------------
}