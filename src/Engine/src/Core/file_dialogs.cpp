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
            pfd::open_file opener(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, pfd::opt::none);

            const auto files = opener.result();
            if (files.empty())
            {
                return std::string();
            }

            return Filesystem::ToString(Utils::U8ToNarrow(files.front()));
        }
        //--------------------------------------------------------------------------

        std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters)
        {
            pfd::open_file opener(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, pfd::opt::multiselect);

            auto paths = std::vector<std::string>();
            const auto files = opener.result();
            if (files.empty())
            {
                return paths;
            }

            for (const auto& path : files)
            {
                paths.push_back(Filesystem::ToString(Utils::U8ToNarrow(path)));
            }

            return paths;
        }
        //--------------------------------------------------------------------------

        std::string OpenDirectory(const std::string& title)
        {
            pfd::open_file opener(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), {}, pfd::opt::force_path);

            const auto directories = opener.result();
            if (directories.empty())
            {
                return std::string();
            }

            return Filesystem::ToString(Utils::U8ToNarrow(directories.front()));
        }
        //--------------------------------------------------------------------------

        std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite)
        {
            pfd::save_file saver(title, Filesystem::ToString(Filesystem::GetCurrentPathCRef()), filters, forceOverwrite ? pfd::opt::force_overwrite : pfd::opt::none);

            return Filesystem::ToString(Utils::U8ToNarrow(saver.result()));
        }
        //--------------------------------------------------------------------------
    }
}
