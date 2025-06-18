#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"

#include <fstream>

#if defined (CreateFile)
    #pragma push_macro("CreateFile")
    #undef CreateFile
    #define KMP_UNDEF_CreateFile
#endif

namespace Kmplete
{
    Path Filesystem::GetCurrentPath() noexcept
    {
        try
        {
            return std::filesystem::current_path();
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR("Filesystem: failed to get current path: '{}'", fe.what());
            return Path{};
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::PathExists(const Path& path) noexcept
    {
        try
        {
            return std::filesystem::exists(path);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR("Filesystem: exists failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::FilePathIsValid(const Path& path) noexcept
    {
        return !path.empty() && path.has_filename();
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreateDirectories(const Path& path, bool pathIsFile /*= false*/) noexcept
    {
        try
        {
            std::filesystem::create_directories(pathIsFile ? path.parent_path() : path);
            return PathExists(pathIsFile ? path.parent_path() : path);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR("Filesystem: create directories failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreateFile(const Path& path) noexcept
    {
        try
        {
            if (PathExists(path))
            {
                return true;
            }

            if (!CreateDirectories(path, true))
            {
                return false;
            }

            std::ofstream(path).flush();
        }
        catch (KMP_MB_UNUSED const std::ios_base::failure& e)
        {
            KMP_LOG_ERROR("Filesystem: failed to create [{}]: '{}'", ToGenericU8String(path), e.what());
        }

        return PathExists(path);
    }
    //--------------------------------------------------------------------------    

    bool Filesystem::IsFile(const Path& path) noexcept
    {
        try
        {
            return std::filesystem::is_regular_file(path);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR("Filesystem: 'isFile' failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::IsDirectory(const Path& path) noexcept
    {
        try
        {
            return std::filesystem::is_directory(path);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR("Filesystem: 'isDirectory' failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToGenericU8String(const Path& path)
    {
        return path.generic_u8string();
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToGenericString(const Path& path)
    {
        return path.generic_string();
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToNativeU8String(const Path& path)
    {
        return path.u8string();
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToNativeString(const Path& path)
    {
        return path.string();
    }
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_CreateFile)
    #pragma pop_macro("CreateFile")
    #undef KMP_UNDEF_CreateFile
#endif
