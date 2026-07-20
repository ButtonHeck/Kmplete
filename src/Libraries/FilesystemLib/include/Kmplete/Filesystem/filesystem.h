#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"


#if defined (CreateFile)
    #pragma push_macro("CreateFile")
    #undef CreateFile
    #define KMP_UNDEF_CreateFile
#endif
#if defined (CopyFile)
    #pragma push_macro("CopyFile")
    #undef CopyFile
    #define KMP_UNDEF_CopyFile
#endif

namespace Kmplete
{
    //! Wrapper of common filesystem-related functions that uses std::filesystem as actual backend
    namespace Filesystem
    {
        KMP_NODISCARD KMP_API Filepath GetCurrentFilepath() noexcept;
        
        KMP_NODISCARD KMP_API bool FilepathExists(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool FilepathIsValid(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool Rename(const Filepath& oldPath, const Filepath& newPath, bool overwrite = false) noexcept;

        KMP_NODISCARD KMP_API bool CreateDirectories(const Filepath& filepath, bool pathIsFile = false) noexcept;
        KMP_NODISCARD KMP_API bool RemoveDirectories(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool CopyDirectories(const Filepath& from, const Filepath& to, std::filesystem::copy_options copyOptions = std::filesystem::copy_options::recursive) noexcept;
        KMP_NODISCARD KMP_API bool IsDirectory(const Filepath& filepath) noexcept;

        KMP_NODISCARD KMP_API bool CreateFile(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool RemoveFile(const Filepath& filepath) noexcept;
        KMP_NODISCARD KMP_API bool CopyFile(const Filepath& from, const Filepath& to, std::filesystem::copy_options copyOptions = std::filesystem::copy_options::skip_existing) noexcept;
        KMP_NODISCARD KMP_API bool IsFile(const Filepath& filepath) noexcept;
        
        KMP_NODISCARD KMP_API String ToGenericU8String(const Filepath& filepath);
        KMP_NODISCARD KMP_API String ToGenericString(const Filepath& filepath);
        KMP_NODISCARD KMP_API String ToNativeU8String(const Filepath& filepath);
        KMP_NODISCARD KMP_API String ToNativeString(const Filepath& filepath);

        KMP_NODISCARD KMP_API String ReadFileAsText(const Filepath& filepath);
        KMP_NODISCARD KMP_API BinaryBuffer ReadFileAsBinary(const Filepath& filepath);

        KMP_NODISCARD KMP_API bool WriteFile(const Filepath& filepath, const String& string, bool append);
        KMP_NODISCARD KMP_API bool WriteFile(const Filepath& filepath, const BinaryBuffer& binaryBuffer, bool append);
        KMP_NODISCARD KMP_API bool WriteFile(const Filepath& filepath, const BinaryBuffer32& binaryBuffer, bool append);
    };
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_CreateFile)
    #pragma pop_macro("CreateFile")
    #undef KMP_UNDEF_CreateFile
#endif
#if defined (KMP_UNDEF_CopyFile)
    #pragma pop_macro("CopyFile")
    #undef KMP_UNDEF_CopyFile
#endif