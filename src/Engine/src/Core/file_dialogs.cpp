#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/string_utils.h"

#include <portable-file-dialogs.h>

namespace Kmplete
{
    namespace FileDialogs
    {
        std::filesystem::path OpenFile(const std::string& title, const std::vector<std::string>& filters)
        {
            pfd::open_file opener(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), filters, pfd::opt::none);

            const auto files = opener.result();
            if (files.empty())
            {
                return std::filesystem::path();
            }

            return std::filesystem::path(Utils::Utf8ToNarrow(files.front()));
        }
        //--------------------------------------------------------------------------

        std::vector<std::filesystem::path> OpenFiles(const std::string& title, const std::vector<std::string>& filters)
        {
            pfd::open_file opener(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), filters, pfd::opt::multiselect);

            auto paths = std::vector<std::filesystem::path>();
            const auto files = opener.result();
            if (files.empty())
            {
                return paths;
            }

            paths.reserve(files.size());
            for (const auto& file : files)
            {
                paths.emplace_back(Utils::Utf8ToNarrow(file));
            }

            return paths;
        }
        //--------------------------------------------------------------------------

        std::filesystem::path OpenDirectory(const std::string& title)
        {
            pfd::select_folder opener(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), pfd::opt::force_path);

            const auto directory = opener.result();
            if (directory.empty())
            {
                return std::filesystem::path();
            }

            return std::filesystem::path(Utils::Utf8ToNarrow(directory));
        }
        //--------------------------------------------------------------------------

        std::filesystem::path SaveFile(const std::string& title, const std::vector<std::string>& filters, bool forceOverwrite)
        {
            pfd::save_file saver(title, Filesystem::ToGenericString(Filesystem::GetApplicationPathCRef()), filters, forceOverwrite ? pfd::opt::force_overwrite : pfd::opt::none);

            return std::filesystem::path(Utils::Utf8ToNarrow(saver.result()));
        }
        //--------------------------------------------------------------------------

        MessageButton OpenMessage(const std::string& title, const std::string& message, MessageChoice choice, MessageIcon icon)
        {
            pfd::choice pfdChoice = pfd::choice::ok_cancel;
            switch (choice)
            {
            case MessageChoice::Ok:
                pfdChoice = pfd::choice::ok;
                break;

            case MessageChoice::OkCancel:
                pfdChoice = pfd::choice::ok_cancel;
                break;

            case MessageChoice::YesNo:
                pfdChoice = pfd::choice::yes_no;
                break;

            case MessageChoice::YesNoCancel:
                pfdChoice = pfd::choice::yes_no_cancel;
                break;

            case MessageChoice::RetryCancel:
                pfdChoice = pfd::choice::retry_cancel;
                break;

            case MessageChoice::AbortRetryIgnore:
                pfdChoice = pfd::choice::abort_retry_ignore;
                break;

            default:
                break;
            }

            pfd::icon pfdIcon = pfd::icon::info;
            switch (icon)
            {
            case MessageIcon::Info:
                pfdIcon = pfd::icon::info;
                break;

            case MessageIcon::Warning:
                pfdIcon = pfd::icon::warning;
                break;

            case MessageIcon::Error:
                pfdIcon = pfd::icon::error;
                break;

            case MessageIcon::Question:
                pfdIcon = pfd::icon::question;
                break;

            default:
                break;
            }

            pfd::message pfdMessage(title, message, pfdChoice, pfdIcon);

            const pfd::button pfdButton = pfdMessage.result();
            switch (pfdButton)
            {
            case pfd::button::cancel:
                return MessageButton::Cancel;

            case pfd::button::ok:
                return MessageButton::Ok;

            case pfd::button::yes:
                return MessageButton::Yes;

            case pfd::button::no:
                return MessageButton::No;

            case pfd::button::abort:
                return MessageButton::Abort;

            case pfd::button::retry:
                return MessageButton::Retry;

            case pfd::button::ignore:
                return MessageButton::Ignore;

            default:
                break;
            }

            return MessageButton::Ignore;
        }
        //--------------------------------------------------------------------------
    }
}
