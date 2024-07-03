#pragma once

#include "kmplete_export.h"

#include <string>
#include <vector>

namespace Kmplete
{
    namespace FileDialogs
    {
        KMPLETE_API std::string OpenFile(const std::string& title, const std::vector<std::string>& filters);
        //--------------------------------------------------------------------------

        KMPLETE_API std::vector<std::string> OpenFiles(const std::string& title, const std::vector<std::string>& filters);
        //--------------------------------------------------------------------------

        KMPLETE_API std::string OpenDirectory(const std::string& title);
        //--------------------------------------------------------------------------

        KMPLETE_API std::string SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite = false);
        //--------------------------------------------------------------------------
    };
}
