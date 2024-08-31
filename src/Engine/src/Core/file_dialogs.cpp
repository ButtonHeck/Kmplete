#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/string_utils.h"

#include <portable-file-dialogs.h>

namespace Kmplete
{
    namespace FileDialogs
    {
        std::string OpenFile(const std::string& title, const std::vector<std::string>& filters)
        {
            pfd::open_file opener(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), filters, pfd::opt::none);

            const auto files = opener.result();
            if (files.empty())
            {
                return std::string();
            }

            return Filesystem::ToGenericString(Utils::Utf8ToNarrow(files.front()));
        }
        //--------------------------------------------------------------------------

        std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters)
        {
            pfd::open_file opener(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), filters, pfd::opt::multiselect);

            auto paths = std::vector<std::string>();
            const auto files = opener.result();
            if (files.empty())
            {
                return paths;
            }

            for (const auto& path : files)
            {
                paths.push_back(Filesystem::ToGenericString(Utils::Utf8ToNarrow(path)));
            }

            return paths;
        }
        //--------------------------------------------------------------------------

        std::string OpenDirectory(const std::string& title)
        {
            pfd::open_file opener(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), {}, pfd::opt::force_path);

            const auto directories = opener.result();
            if (directories.empty())
            {
                return std::string();
            }

            return Filesystem::ToGenericString(Utils::Utf8ToNarrow(directories.front()));
        }
        //--------------------------------------------------------------------------

        std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite)
        {
            pfd::save_file saver(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), filters, forceOverwrite ? pfd::opt::force_overwrite : pfd::opt::none);

            return Filesystem::ToGenericString(Utils::Utf8ToNarrow(saver.result()));
        }
        //--------------------------------------------------------------------------
    }
}
