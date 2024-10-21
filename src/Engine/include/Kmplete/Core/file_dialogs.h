#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/types.h"

#include <string>
#include <vector>
#include <filesystem>

namespace Kmplete
{
    namespace FileDialogs
    {
        KMP_NODISCARD KMP_API std::filesystem::path OpenFile(const std::string& title, const StringVector& filters = { "Any Files", "*.*" });
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API std::vector<std::filesystem::path> OpenFiles(const std::string& title, const StringVector& filters = { "Any Files", "*.*" });
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API std::filesystem::path OpenDirectory(const std::string& title);
        //--------------------------------------------------------------------------

        KMP_NODISCARD KMP_API std::filesystem::path SaveFile(const std::string& title, const StringVector& filters = { "Any Files", "*.*" }, bool forceOverwrite = false);
        //--------------------------------------------------------------------------

        enum class MessageButton
        {
            Cancel = -1,
            Ok,
            Yes,
            No,
            Abort,
            Retry,
            Ignore
        };

        enum class MessageChoice
        {
            Ok = 0,
            OkCancel,
            YesNo,
            YesNoCancel,
            RetryCancel,
            AbortRetryIgnore
        };

        enum class MessageIcon
        {
            Info = 0,
            Warning,
            Error,
            Question
        };

        KMP_NODISCARD KMP_API MessageButton OpenMessage(const std::string& title, const std::string& message, MessageChoice choice = MessageChoice::OkCancel, MessageIcon = MessageIcon::Info);
        //--------------------------------------------------------------------------
    };
}
