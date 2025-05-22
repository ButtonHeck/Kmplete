#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Core/types.h"

#include <filesystem>

using Path = std::filesystem::path;

namespace Kmplete
{
    class Filesystem
    {
    public:
        KMP_NODISCARD KMP_API static bool Initialize();

        KMP_NODISCARD KMP_API static Path GetApplicationPath() noexcept;
        KMP_NODISCARD KMP_API static const Path& GetApplicationPathCRef() noexcept;
        KMP_NODISCARD KMP_API static Path GetCurrentPath() noexcept;
        
        KMP_NODISCARD KMP_API static bool PathExists(const Path& path) noexcept;
        KMP_NODISCARD KMP_API static bool FilePathIsValid(const Path& path) noexcept;
        KMP_NODISCARD KMP_API static bool CreateDirectories(const Path& path, bool pathIsFile = false) noexcept;
        KMP_NODISCARD KMP_API static bool CreateFile(const Path& path) noexcept;
        
        KMP_NODISCARD KMP_API static bool IsFile(const Path& path) noexcept;
        KMP_NODISCARD KMP_API static bool IsDirectory(const Path& path) noexcept;
        
        KMP_NODISCARD KMP_API static String ToGenericU8String(const Path& path);
        KMP_NODISCARD KMP_API static String ToGenericString(const Path& path);
        KMP_NODISCARD KMP_API static String ToNativeU8String(const Path& path);
        KMP_NODISCARD KMP_API static String ToNativeString(const Path& path);

    private:
        static Path _applicationPath;
    };
    //--------------------------------------------------------------------------
}
