#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Utils/string_utils.h"

#include <portable-file-dialogs.h>

namespace Kmplete
{
    static constexpr auto StringToPathConverterFn = Utils::Utf8ToNarrow;
    static constexpr auto PathToStringConverterFn = Filesystem::ToGenericString;

    namespace FileDialogs
    {
        Path OpenFile(const std::string& title, const StringVector& filters)
        {
            pfd::open_file opener(title, PathToStringConverterFn(Filesystem::GetApplicationPathCRef()), filters, pfd::opt::none);

            const auto files = opener.result();
            if (files.empty())
            {
                return Path();
            }

            const auto file = StringToPathConverterFn(files.front());
            return Path(Utils::NarrowToWide(file));
        }
        //--------------------------------------------------------------------------

        std::vector<Path> OpenFiles(const std::string& title, const StringVector& filters)
        {
            pfd::open_file opener(title, PathToStringConverterFn(Filesystem::GetApplicationPathCRef()), filters, pfd::opt::multiselect);

            auto paths = std::vector<Path>();
            const auto files = opener.result();
            if (files.empty())
            {
                return paths;
            }

            paths.reserve(files.size());
            for (const auto& file : files)
            {
                const auto fileToEmplace = StringToPathConverterFn(file);
                paths.emplace_back(Utils::NarrowToWide(fileToEmplace));
            }

            return paths;
        }
        //--------------------------------------------------------------------------

        Path OpenDirectory(const std::string& title)
        {
            pfd::select_folder opener(title, PathToStringConverterFn(Filesystem::GetApplicationPathCRef()), pfd::opt::force_path);

            auto directory = opener.result();
            if (directory.empty())
            {
                return Path();
            }

            directory = StringToPathConverterFn(directory);
            return Path(Utils::NarrowToWide(directory));
        }
        //--------------------------------------------------------------------------

        Path SaveFile(const std::string& title, const StringVector& filters, bool forceOverwrite)
        {
            pfd::save_file saver(title, PathToStringConverterFn(Filesystem::GetApplicationPathCRef()), filters, forceOverwrite ? pfd::opt::force_overwrite : pfd::opt::none);

            auto dialogResult = saver.result();
            dialogResult = StringToPathConverterFn(dialogResult);
            return Path(Utils::NarrowToWide(dialogResult));
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
