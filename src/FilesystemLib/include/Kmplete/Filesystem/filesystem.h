#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types.h"

#include <spdlog/fmt/fmt.h>

namespace Kmplete
{
    namespace Filesystem
    {
        KMP_NODISCARD KMP_API Path GetCurrentPath() noexcept;
        
        KMP_NODISCARD KMP_API bool PathExists(const Path& path) noexcept;
        KMP_NODISCARD KMP_API bool FilePathIsValid(const Path& path) noexcept;
        KMP_NODISCARD KMP_API bool CreateDirectories(const Path& path, bool pathIsFile = false) noexcept;
        KMP_NODISCARD KMP_API bool RemoveDirectories(const Path& path) noexcept;
        KMP_NODISCARD KMP_API bool CreateFile(const Path& path) noexcept;
        KMP_NODISCARD KMP_API bool RemoveFile(const Path& path) noexcept;
        
        KMP_NODISCARD KMP_API bool IsFile(const Path& path) noexcept;
        KMP_NODISCARD KMP_API bool IsDirectory(const Path& path) noexcept;
        
        KMP_NODISCARD KMP_API String ToGenericU8String(const Path& path);
        KMP_NODISCARD KMP_API String ToGenericString(const Path& path);
        KMP_NODISCARD KMP_API String ToNativeU8String(const Path& path);
        KMP_NODISCARD KMP_API String ToNativeString(const Path& path);
    };
    //--------------------------------------------------------------------------
}

template<>
struct fmt::formatter<Kmplete::Path> : fmt::formatter<Kmplete::String>
{
    template <typename FormatContext>
    auto format(const Kmplete::Path& path, FormatContext& ctx)
    {
        return fmt::formatter<Kmplete::String>::format(Kmplete::Filesystem::ToGenericU8String(path), ctx);
    }
};
//--------------------------------------------------------------------------
