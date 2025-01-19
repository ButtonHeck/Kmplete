#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <filesystem>

using Path = std::filesystem::path;

namespace Kmplete
{
    class Filesystem
    {
    public:
        KMP_NODISCARD KMP_API static bool Initialize();

        KMP_NODISCARD KMP_API static Path GetApplicationPath() KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static const Path& GetApplicationPathCRef() KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static Path GetCurrentPath() KMP_NOEXCEPT;
        
        KMP_NODISCARD KMP_API static bool PathExists(const Path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool FilePathIsValid(const Path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool CreateDirectories(const Path& path, bool pathIsFile = false) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool CreateFile(const Path& path) KMP_NOEXCEPT;
        
        KMP_NODISCARD KMP_API static bool IsFile(const Path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool IsDirectory(const Path& path) KMP_NOEXCEPT;
        
        KMP_NODISCARD KMP_API static std::string ToGenericU8String(const Path& path);
        KMP_NODISCARD KMP_API static std::string ToGenericString(const Path& path);
        KMP_NODISCARD KMP_API static std::string ToNativeU8String(const Path& path);
        KMP_NODISCARD KMP_API static std::string ToNativeString(const Path& path);

    private:
        static Path _applicationPath;
    };
    //--------------------------------------------------------------------------
}
