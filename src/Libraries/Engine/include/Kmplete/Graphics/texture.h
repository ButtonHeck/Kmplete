#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


namespace Kmplete
{
    namespace Graphics
    {
        //! Base class for texture objects backed by graphics API
        class Texture
        {
            KMP_DISABLE_COPY_MOVE(Texture)

        public:
            Texture() = default;
            virtual ~Texture() = default;
        };
        //--------------------------------------------------------------------------
    }
}