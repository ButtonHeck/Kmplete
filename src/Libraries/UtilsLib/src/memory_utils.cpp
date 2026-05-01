#include "Kmplete/Utils/memory_utils.h"
#include "Kmplete/Base/platform.h"
#include "Kmplete/Log/log.h"

#include <cstdlib>


namespace Kmplete
{
    namespace Utils
    {
        void* AlignedAlloc(size_t size, size_t alignment)
        {
            if (alignment == 0)
            {
                KMP_LOG_ERROR_FN("Utils: AlignedAlloc - cannot allocate with alignment 0");
                return nullptr;
            }
            if (size == 0)
            {
                KMP_LOG_ERROR_FN("Utils: AlignedAlloc - cannot allocate with size 0");
                return nullptr;
            }

            void* data = nullptr;
#if defined (KMP_COMPILER_MSVC) || defined (KMP_COMPILER_MINGW)
            data = _aligned_malloc(size, alignment);
#else
            int res = posix_memalign(&data, alignment, size);
            if (res != 0)
            {
                data = nullptr;
            }
#endif
            return data;
        }
        //--------------------------------------------------------------------------

        void AlignedFree(void* data)
        {
#if defined (KMP_COMPILER_MSVC) || defined (KMP_COMPILER_MINGW)
            _aligned_free(data);
#else
            free(data);
#endif
        }
        //--------------------------------------------------------------------------
    }
}