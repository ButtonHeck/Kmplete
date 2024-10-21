#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/log.h"

#include <fstream>

#ifdef CreateFile
#pragma push_macro("CreateFile")
#undef CreateFile
#define KMP_UNDEF_CreateFile
#endif

namespace Kmplete
{
    std::filesystem::path Filesystem::_applicationPath;

    bool Filesystem::Initialize()
    {
        try
        {
            _applicationPath = std::filesystem::current_path();
            return true;
        }
        catch (const std::filesystem::filesystem_error& fe)
        {
            Log::CoreError("Filesystem: failed to initialize application path: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    std::filesystem::path Filesystem::GetApplicationPath() KMP_NOEXCEPT
    {
        return _applicationPath;
    }
    //--------------------------------------------------------------------------

    const std::filesystem::path& Filesystem::GetApplicationPathCRef() KMP_NOEXCEPT
    {
        return _applicationPath;
    }
    //--------------------------------------------------------------------------

    std::filesystem::path Filesystem::GetCurrentPath() KMP_NOEXCEPT
    {
        try
        {
            return std::filesystem::current_path();
        }
        catch (const std::filesystem::filesystem_error& fe)
        {
            Log::CoreError("Filesystem: failed to get current path: '{}'", fe.what());
            return std::filesystem::path();
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::PathExists(const std::filesystem::path& path) KMP_NOEXCEPT
    {
        try
        {
            return std::filesystem::exists(path);
        }
        catch (const std::filesystem::filesystem_error& fe)
        {
            Log::CoreError("Filesystem: exists failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::FilePathIsValid(const std::filesystem::path& path) KMP_NOEXCEPT
    {
        return !path.empty() && path.has_filename();
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreateDirectories(const std::filesystem::path& path, bool pathIsFile) KMP_NOEXCEPT
    {
        try
        {
            std::filesystem::create_directories(pathIsFile ? path.parent_path() : path);
            return PathExists(pathIsFile ? path.parent_path() : path);
        }
        catch (const std::filesystem::filesystem_error& fe)
        {
            Log::CoreError("Filesystem: create directories failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreateFile(const std::filesystem::path& path) KMP_NOEXCEPT
    {
        try
        {
            if (!CreateDirectories(path, true))
            {
                return false;
            }

            std::ofstream(path).flush();
        }
        catch (const std::ios_base::failure& e)
        {
            Log::CoreError("Filesystem: failed to create [{}]: '{}'", ToGenericU8String(path), e.what());
        }

        return PathExists(path);
    }
    //--------------------------------------------------------------------------    

    bool Filesystem::IsFile(const std::filesystem::path& path) KMP_NOEXCEPT
    {
        try
        {
            return std::filesystem::is_regular_file(path);
        }
        catch (const std::filesystem::filesystem_error& fe)
        {
            Log::Error("Filesystem: 'isFile' failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::IsDirectory(const std::filesystem::path& path) KMP_NOEXCEPT
    {
        try
        {
            return std::filesystem::is_directory(path);
        }
        catch (const std::filesystem::filesystem_error& fe)
        {
            Log::Error("Filesystem: 'isDirectory' failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::ToGenericU8String(const std::filesystem::path& path)
    {
        return path.generic_u8string();
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::ToGenericString(const std::filesystem::path& path)
    {
        return path.generic_string();
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::ToNativeU8String(const std::filesystem::path& path)
    {
        return path.u8string();
    }
    //--------------------------------------------------------------------------

    std::string Filesystem::ToNativeString(const std::filesystem::path& path)
    {
        return path.string();
    }
    //--------------------------------------------------------------------------
}

#ifdef KMP_UNDEF_CreateFile
#pragma pop_macro("CreateFile")
#undef KMP_UNDEF_CreateFile
#endif