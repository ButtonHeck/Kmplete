#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <filesystem>

namespace Kmplete
{
    class Filesystem
    {
    public:
        KMP_API KMP_NODISCARD static bool Initialize();

        KMP_API KMP_NODISCARD static std::filesystem::path GetApplicationPath() KMP_NOEXCEPT;
        KMP_API KMP_NODISCARD static const std::filesystem::path& GetApplicationPathCRef() KMP_NOEXCEPT;
        KMP_API KMP_NODISCARD static std::filesystem::path GetCurrentPath() KMP_NOEXCEPT;

        KMP_API KMP_NODISCARD static bool PathExists(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_API KMP_NODISCARD static bool FilePathIsValid(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_API KMP_NODISCARD static bool CreateDirectories(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_API KMP_NODISCARD static bool CreateFile(const std::filesystem::path& path) KMP_NOEXCEPT;

        KMP_API KMP_NODISCARD static bool IsFile(const std::filesystem::path& path) KMP_NOEXCEPT;
        KMP_API KMP_NODISCARD static bool IsDirectory(const std::filesystem::path& path) KMP_NOEXCEPT;

        KMP_API KMP_NODISCARD static std::string ToGenericU8String(const std::filesystem::path& path);
        KMP_API KMP_NODISCARD static std::string ToGenericString(const std::filesystem::path& path);
        KMP_API KMP_NODISCARD static std::string ToNativeU8String(const std::filesystem::path& path);
        KMP_API KMP_NODISCARD static std::string ToNativeString(const std::filesystem::path& path);

    private:
        static std::filesystem::path _applicationPath;
    };
    //--------------------------------------------------------------------------
}
