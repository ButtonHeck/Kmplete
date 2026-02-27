#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/nullability.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class for texture objects backed by graphics API
        class Texture
        {
            KMP_DISABLE_COPY_MOVE(Texture)

        public:
            KMP_API Texture() noexcept;
            virtual ~Texture() = default;

            KMP_NODISCARD KMP_API Nullable<void*> GetHandle() const;

        protected:
            UInt64 _handle;
        };
        //--------------------------------------------------------------------------
    }
}