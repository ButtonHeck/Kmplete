#pragma once

#include "Kmplete/Base/kmplete_api.h"


namespace Kmplete
{
    namespace Utils
    {
        //! Plaform abstraction for aligned memory allocation, use AlignedFree for this memory
        KMP_NODISCARD KMP_API void* AlignedAlloc(size_t size, size_t alignment);

        //! Platform abstraction for aligned memory deallocation, use only for AlignedAlloc-ed memory
        KMP_API void AlignedFree(void* data);
    }
}