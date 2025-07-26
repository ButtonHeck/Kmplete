#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

namespace Kmplete
{
    namespace Filesystem
    {
        KMP_NODISCARD KMP_API Filepath GetCurrentFilepath() noexcept;
        
        KMP_NODISCARD KMP_API bool FilepathExists(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool FilepathIsValid(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool CreateDirectories(const Filepath& filepath, bool pathIsFile = false) noexcept;
        KMP_NODISCARD KMP_API bool RemoveDirectories(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool CreateFile(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool RemoveFile(const Filepath& filepath) noexcept;
        
        KMP_NODISCARD KMP_API bool IsFile(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool IsDirectory(const Filepath& filepath) noexcept;
        
        KMP_NODISCARD KMP_API String ToGenericU8String(const Filepath& filepath);
        KMP_NODISCARD KMP_API String ToGenericString(const Filepath& filepath);
        KMP_NODISCARD KMP_API String ToNativeU8String(const Filepath& filepath);
        KMP_NODISCARD KMP_API String ToNativeString(const Filepath& filepath);
    };
    //--------------------------------------------------------------------------
}
