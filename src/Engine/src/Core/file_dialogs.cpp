#include "Kmplete/Core/file_dialogs.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Utils/string_utils.h"

#include <portable-file-dialogs.h>

namespace Kmplete
{
    static constexpr auto StringToFilepathConverterFn = Utils::Utf8ToNarrow;
    static constexpr auto FilepathToStringConverterFn = Filesystem::ToGenericString;

    namespace FileDialogs
    {
        Filepath OpenFile(const String& title, const Filepath& startFilepath, const StringVector& filters /*= { "Any Files", "*.*" }*/)
        {
            pfd::open_file opener(title, FilepathToStringConverterFn(startFilepath), filters, pfd::opt::none);

            const auto files = opener.result();
            if (files.empty())
            {
                return Filepath();
            }

            const auto file = StringToFilepathConverterFn(files.front());
            return Filepath(Utils::NarrowToWide(file));
        }
        //--------------------------------------------------------------------------

        FilepathVector OpenFiles(const String& title, const Filepath& startFilepath, const StringVector& filters /*= { "Any Files", "*.*" }*/)
        {
            pfd::open_file opener(title, FilepathToStringConverterFn(startFilepath), filters, pfd::opt::multiselect);

            auto filepaths = FilepathVector();
            const auto files = opener.result();
            if (files.empty())
            {
                return filepaths;
            }

            filepaths.reserve(files.size());
            for (const auto& file : files)
            {
                const auto fileToEmplace = StringToFilepathConverterFn(file);
                filepaths.emplace_back(Utils::NarrowToWide(fileToEmplace));
            }

            return filepaths;
        }
        //--------------------------------------------------------------------------

        Filepath OpenDirectory(const String& title, const Filepath& startFilepath)
        {
            pfd::select_folder opener(title, FilepathToStringConverterFn(startFilepath), pfd::opt::force_path);

            auto directory = opener.result();
            if (directory.empty())
            {
                return Filepath();
            }

            directory = StringToFilepathConverterFn(directory);
            return Filepath(Utils::NarrowToWide(directory));
        }
        //--------------------------------------------------------------------------

        Filepath SaveFile(const String& title, const Filepath& startFilepath, const StringVector& filters /*= { "Any Files", "*.*" }*/, bool forceOverwrite /*= false*/)
        {
            pfd::save_file saver(title, FilepathToStringConverterFn(startFilepath), filters, forceOverwrite ? pfd::opt::force_overwrite : pfd::opt::none);

            auto dialogResult = saver.result();
            dialogResult = StringToFilepathConverterFn(dialogResult);
            return Filepath(Utils::NarrowToWide(dialogResult));
        }
        //--------------------------------------------------------------------------

        MessageButton OpenMessage(const String& title, const String& message, MessageChoice choice /*= MessageChoice::OkCancel*/, MessageIcon icon /*= MessageIcon::Info*/)
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
