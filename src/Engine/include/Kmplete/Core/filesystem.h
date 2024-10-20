#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <filesystem>

namespace Kmplete
{
    class Filesystem
    {
    public:
        KMP_NODISCARD KMP_API static bool Initialize();

        KMP_NODISCARD KMP_API static std::filesystem::path GetApplicationPath() KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static const std::filesystem::path& GetApplicationPathCRef() KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static std::filesystem::path GetCurrentPath() KMP_NOEXCEPT;
        
        KMP_NODISCARD KMP_API static bool PathExists(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool FilePathIsValid(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool CreateDirectories(const std::filesystem::path& path, bool pathIsFile = false) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool CreateFile(const std::filesystem::path& path) KMP_NOEXCEPT;
        
        KMP_NODISCARD KMP_API static bool IsFile(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_NODISCARD KMP_API static bool IsDirectory(const std::filesystem::path& path) KMP_NOEXCEPT;
        
        KMP_NODISCARD KMP_API static std::string ToGenericU8String(const std::filesystem::path& path);
        KMP_NODISCARD KMP_API static std::string ToGenericString(const std::filesystem::path& path);
        KMP_NODISCARD KMP_API static std::string ToNativeU8String(const std::filesystem::path& path);
        KMP_NODISCARD KMP_API static std::string ToNativeString(const std::filesystem::path& path);

    private:
        static std::filesystem::path _applicationPath;
    };
    //--------------------------------------------------------------------------
}
