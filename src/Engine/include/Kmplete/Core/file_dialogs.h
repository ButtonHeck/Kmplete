#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"

namespace Kmplete
{
    namespace FileDialogs
    {
        KMP_NODISCARD KMP_API Filepath OpenFile(const String& title, const Filepath& startFilepath, const StringVector& filters = { "Any Files", "*.*" });
        KMP_NODISCARD KMP_API FilepathVector OpenFiles(const String& title, const Filepath& startFilepath, const StringVector& filters = { "Any Files", "*.*" });
        KMP_NODISCARD KMP_API Filepath OpenDirectory(const String& title, const Filepath& startFilepath);
        KMP_NODISCARD KMP_API Filepath SaveFile(const String& title, const Filepath& startFilepath, const StringVector& filters = { "Any Files", "*.*" }, bool forceOverwrite = false);

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

        KMP_NODISCARD KMP_API MessageButton OpenMessage(const String& title, const String& message, 
                                                        MessageChoice choice = MessageChoice::OkCancel, MessageIcon icon = MessageIcon::Info);
    };
}
