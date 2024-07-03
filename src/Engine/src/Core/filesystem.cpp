#include "Kmplete/Core/filesystem.h"

namespace Kmplete
{
    std::filesystem::path Filesystem::_currentPath;

    void Filesystem::Initialize()
    {
        _currentPath = std::filesystem::current_path();
    }
    //--------------------------------------------------------------------------

    std::filesystem::path Filesystem::GetCurrentPath()
    {
        return _currentPath;
    }
    //--------------------------------------------------------------------------

    const std::filesystem::path& Filesystem::GetCurrentPathCRef()
    {
        return _currentPath;
    }
    //--------------------------------------------------------------------------

    bool Filesystem::PathExists(const std::filesystem::path& path)
    {
        return std::filesystem::exists(path);
    }
    //--------------------------------------------------------------------------

    bool Filesystem::FilePathIsValid(const std::filesystem::path& path)
    {
        return !path.empty() && path.has_filename() && path.has_extension();
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreatePathTree(const std::filesystem::path& path)
    {
        if (!FilePathIsValid(path))
        {
            return false;
        }

        std::filesystem::create_directories(path.parent_path());

        return true;
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::ToU8String(const std::filesystem::path& path)
    {
        return path.generic_u8string();
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::ToString(const std::filesystem::path& path)
    {
        return path.generic_string();
    }
    //--------------------------------------------------------------------------
}
