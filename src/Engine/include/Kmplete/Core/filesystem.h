#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <filesystem>

namespace Kmplete
{
    class KMP_API Filesystem
    {
    public:
        static void Initialize();

        KMP_NODISCARD static std::filesystem::path GetCurrentPath();
        KMP_NODISCARD static const std::filesystem::path& GetCurrentPathCRef();

        KMP_NODISCARD static bool PathExists(const std::filesystem::path& path);
        KMP_NODISCARD static bool FilePathIsValid(const std::filesystem::path& path);
        KMP_NODISCARD static bool CreatePathTree(const std::filesystem::path& path);

        KMP_NODISCARD static std::string ToU8String(const std::filesystem::path& path);
        KMP_NODISCARD static std::string ToString(const std::filesystem::path& path);

    private:
        static std::filesystem::path _currentPath;
    };
    //--------------------------------------------------------------------------
}
