#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <string>
#include <vector>

namespace Kmplete
{
    namespace FileDialogs
    {
        KMP_NODISCARD KMP_API std::string OpenFile(const std::string& title, const std::vector<std::string>& filters = { "Any Files", "*.*" });
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters = { "Any Files", "*.*" });
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API std::string OpenDirectory(const std::string& title);
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API std::string SaveFile(const std::string& title, const std::vector<std::string>& filters = { "Any Files", "*.*" }, bool forceOverwrite = false);
        //--------------------------------------------------------------------------
    };
}
