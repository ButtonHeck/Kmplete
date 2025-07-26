#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

namespace Kmplete
{
    namespace Filesystem
    {
        KMP_NODISCARD KMP_API Filepath GetCurrentPath() noexcept;
        
        KMP_NODISCARD KMP_API bool PathExists(const Filepath& path) noexcept;
        KMP_NODISCARD KMP_API bool FilePathIsValid(const Filepath& path) noexcept;
        KMP_NODISCARD KMP_API bool CreateDirectories(const Filepath& path, bool pathIsFile = false) noexcept;
        KMP_NODISCARD KMP_API bool RemoveDirectories(const Filepath& path) noexcept;
        KMP_NODISCARD KMP_API bool CreateFile(const Filepath& path) noexcept;
        KMP_NODISCARD KMP_API bool RemoveFile(const Filepath& path) noexcept;
        
        KMP_NODISCARD KMP_API bool IsFile(const Filepath& path) noexcept;
        KMP_NODISCARD KMP_API bool IsDirectory(const Filepath& path) noexcept;
        
        KMP_NODISCARD KMP_API String ToGenericU8String(const Filepath& path);
        KMP_NODISCARD KMP_API String ToGenericString(const Filepath& path);
        KMP_NODISCARD KMP_API String ToNativeU8String(const Filepath& path);
        KMP_NODISCARD KMP_API String ToNativeString(const Filepath& path);
    };
    //--------------------------------------------------------------------------
}
