#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <filesystem>

namespace Kmplete
{
    class KMP_API Filesystem
    {
    public:
        static void Initialize();

        static std::filesystem::path GetCurrentPath();
        static const std::filesystem::path& GetCurrentPathCRef();

        static bool PathExists(const std::filesystem::path& path);
        static bool FilePathIsValid(const std::filesystem::path& path);
        static bool CreatePathTree(const std::filesystem::path& path);

        static std::string ToU8String(const std::filesystem::path& path);
        static std::string ToString(const std::filesystem::path& path);

    private:
        static std::filesystem::path _currentPath;
    };
    //--------------------------------------------------------------------------
}
