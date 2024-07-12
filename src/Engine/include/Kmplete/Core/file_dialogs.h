#pragma once

#include "Kmplete/Core/kmplete_api.h"

#include <string>
#include <vector>

namespace Kmplete
{
    namespace FileDialogs
    {
        KMP_API KMP_NODISCARD std::string OpenFile(const std::string& title, const std::vector<std::string>& filters);
        //--------------------------------------------------------------------------

        KMP_API KMP_NODISCARD std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters);
        //--------------------------------------------------------------------------

        KMP_API KMP_NODISCARD std::string OpenDirectory(const std::string& title);
        //--------------------------------------------------------------------------

        KMP_API KMP_NODISCARD std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite = false);
        //--------------------------------------------------------------------------
    };
}
