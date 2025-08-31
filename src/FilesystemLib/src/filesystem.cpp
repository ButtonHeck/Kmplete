#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"

#include <fstream>

#if defined (CreateFile)
    #pragma push_macro("CreateFile")
    #undef CreateFile
    #define KMP_UNDEF_CreateFile
#endif

namespace Kmplete
{
    Filepath Filesystem::GetCurrentFilepath() noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            return std::filesystem::current_path();
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: failed to get current filepath: '{}'", fe.what());
            return Filepath{};
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::FilepathExists(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            return std::filesystem::exists(filepath);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: exists failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::FilepathIsValid(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        return !filepath.empty() && filepath.has_filename();
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreateDirectories(const Filepath& filepath, bool pathIsFile /*= false*/) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            std::filesystem::create_directories(pathIsFile ? filepath.parent_path() : filepath);
            return FilepathExists(pathIsFile ? filepath.parent_path() : filepath);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: create directories failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::RemoveDirectories(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            return std::filesystem::remove_all(filepath) != 0;
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: remove directories failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::CreateFile(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            if (FilepathExists(filepath))
            {
                KMP_LOG_INFO_FN("Filesystem::CreateFile - file '{}' already exists", filepath);
                return true;
            }

            if (!CreateDirectories(filepath, true))
            {
                KMP_LOG_ERROR_FN("Filesystem::CreateFile - cannot create directories for file '{}'", filepath);
                return false;
            }

            std::ofstream(filepath).flush();
        }
        catch (KMP_MB_UNUSED const std::ios_base::failure& e)
        {
            KMP_LOG_ERROR_FN("Filesystem: failed to create [{}]: '{}'", filepath, e.what());
        }

        return FilepathExists(filepath);
    }
    //--------------------------------------------------------------------------

    bool Filesystem::RemoveFile(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            return std::filesystem::remove(filepath);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: remove file failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::IsFile(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            return std::filesystem::is_regular_file(filepath);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: 'isFile' failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    bool Filesystem::IsDirectory(const Filepath& filepath) noexcept
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            return std::filesystem::is_directory(filepath);
        }
        catch (KMP_MB_UNUSED const std::filesystem::filesystem_error& fe)
        {
            KMP_LOG_ERROR_FN("Filesystem: 'isDirectory' failed: '{}'", fe.what());
            return false;
        }
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToGenericU8String(const Filepath& filepath)
    {
        KMP_PROFILE_FUNCTION();

        return filepath.generic_u8string();
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToGenericString(const Filepath& filepath)
    {
        KMP_PROFILE_FUNCTION();

        return filepath.generic_string();
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToNativeU8String(const Filepath& filepath)
    {
        KMP_PROFILE_FUNCTION();

        return filepath.u8string();
    }
    //--------------------------------------------------------------------------

    String Filesystem::ToNativeString(const Filepath& filepath)
    {
        KMP_PROFILE_FUNCTION();

        return filepath.string();
    }
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_CreateFile)
    #pragma pop_macro("CreateFile")
    #undef KMP_UNDEF_CreateFile
#endif
