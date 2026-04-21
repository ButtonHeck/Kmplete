#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Base/named_bool.h"
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
    namespace Filesystem
    {
        //! Codestyle friendly alias for filesystem error
        using FilesystemError = std::filesystem::filesystem_error;


        Filepath GetCurrentFilepath() noexcept KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            try
            {
                return std::filesystem::current_path();
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: failed to get current filepath: '{}'", fe.what());
                return Filepath{};
            }
        }}
        //--------------------------------------------------------------------------

        bool FilepathExists(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            try
            {
                return std::filesystem::exists(filepath);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: exists failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool FilepathIsValid(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            return !filepath.empty() && filepath.has_filename();
        }}
        //--------------------------------------------------------------------------

        bool CreateDirectories(const Filepath& filepath, bool pathIsFile /*= false*/) noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            try
            {
                std::filesystem::create_directories(pathIsFile ? filepath.parent_path() : filepath);
                return FilepathExists(pathIsFile ? filepath.parent_path() : filepath);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: create directories failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool RemoveDirectories(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            try
            {
                return std::filesystem::remove_all(filepath) != 0;
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: remove directories failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool CopyDirectories(const Filepath& from, const Filepath& to, std::filesystem::copy_options copyOptions /*= std::filesystem::copy_options::recursive*/) noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (!IsDirectory(from))
            {
                KMP_LOG_ERROR_FN("Filesystem: CopyDirectories failed: '{}' is not a directory", from);
                return false;
            }

            try
            {
                std::filesystem::copy(from, to, copyOptions);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: CopyDirectories failed: '{}'", fe.what());
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------

        bool IsDirectory(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            try
            {
                return std::filesystem::is_directory(filepath);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: 'isDirectory' failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool CreateFile(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            try
            {
                if (FilepathExists(filepath))
                {
                    KMP_LOG_INFO_FN("CreateFile - file '{}' already exists", filepath);
                    return true;
                }

                if (!CreateDirectories(filepath, "path is file"_true))
                {
                    KMP_LOG_ERROR_FN("CreateFile - cannot create directories for file '{}'", filepath);
                    return false;
                }

                std::ofstream(filepath).flush();
            }
            catch (KMP_MB_UNUSED const std::ios_base::failure& e)
            {
                KMP_LOG_ERROR_FN("Filesystem: failed to create [{}]: '{}'", filepath, e.what());
            }

            return FilepathExists(filepath);
        }}
        //--------------------------------------------------------------------------

        bool RemoveFile(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            try
            {
                return std::filesystem::remove(filepath);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: remove file failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool CopyFile(const Filepath& from, const Filepath& to, std::filesystem::copy_options copyOptions /*= std::filesystem::copy_options::skip_existing*/) noexcept KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            try
            {
                return std::filesystem::copy_file(from, to, copyOptions);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: copy file failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        bool IsFile(const Filepath& filepath) noexcept KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            try
            {
                return std::filesystem::is_regular_file(filepath);
            }
            catch (KMP_MB_UNUSED const FilesystemError& fe)
            {
                KMP_LOG_ERROR_FN("Filesystem: 'isFile' failed: '{}'", fe.what());
                return false;
            }
        }}
        //--------------------------------------------------------------------------

        String ToGenericU8String(const Filepath& filepath) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            return filepath.generic_u8string();
        }}
        //--------------------------------------------------------------------------

        String ToGenericString(const Filepath& filepath) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            return filepath.generic_string();
        }}
        //--------------------------------------------------------------------------

        String ToNativeU8String(const Filepath& filepath) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            return filepath.u8string();
        }}
        //--------------------------------------------------------------------------

        String ToNativeString(const Filepath& filepath) KMP_PROFILING(ProfileLevelMinorVerbose)
        {
            return filepath.string();
        }}
        //--------------------------------------------------------------------------

        String ReadFileAsText(const Filepath& filepath) KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (!FilepathExists(filepath))
            {
                KMP_LOG_ERROR_FN("Filesystem: cannot read file '{}' - not found", filepath);
                return String();
            }

            std::ifstream fileStream(filepath);
            if (!fileStream.is_open() || !fileStream.good())
            {
                KMP_LOG_ERROR_FN("Filesystem: cannot read file '{}' - failed to open", filepath);
                return String();
            }

            return String((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));
        }}
        //--------------------------------------------------------------------------

        BinaryBuffer ReadFileAsBinary(const Filepath& filepath) KMP_PROFILING(ProfileLevelImportantVerbose)
        {
            if (!FilepathExists(filepath))
            {
                KMP_LOG_ERROR_FN("Filesystem: cannot read file '{}' - not found", filepath);
                return BinaryBuffer();
            }

            std::ifstream fileStream(filepath, std::ios::binary);
            if (!fileStream.is_open() || !fileStream.good())
            {
                KMP_LOG_ERROR_FN("Filesystem: cannot read file '{}' - failed to open", filepath);
                return BinaryBuffer();
            }

            return BinaryBuffer((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));
        }}
        //--------------------------------------------------------------------------
    }
}

#if defined (KMP_UNDEF_CreateFile)
    #pragma pop_macro("CreateFile")
    #undef KMP_UNDEF_CreateFile
#endif
