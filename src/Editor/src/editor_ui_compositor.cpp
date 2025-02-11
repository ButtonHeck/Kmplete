#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "shortcuts.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Localization/localization_dictionary.h"
#include "Kmplete/Localization/localization_translator.h"

#include <imgui.h>
#include <imgui_internal.h> // for ImGui::DockBuilder api
#include <misc/cpp/imgui_stdlib.h> // for ImGui::InputText wrappers for std::string

namespace Kmplete
{
    EditorUICompositor::EditorUICompositor(const Ptr<Window> window, const Ptr<LocalizationManager> localizationManager, const Ptr<LocalizationDictionary> localizationDict)
        : _window(window)
        , _localizationManager(localizationManager)
        , _localizationDict(localizationDict)
    {
        FillDictionary();
        _localizationManager->AddLocaleChangedCallback(KMP_BIND(EditorUICompositor::FillDictionary));
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMainArea()
    {
        if (!Filesystem::PathExists(Filesystem::GetCurrentPath().append(ImGui::GetIO().IniFilename)))
        {
            ComposeDefaultLayout();
        }

        ComposeMenu();

        ComposePopups();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeDefaultLayout()
    {
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            ComposeMenuFile();
            ComposeMenuView();
            ComposeMenuLanguage();

            ImGui::EndMenuBar();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFile()
    {
        if (ImGui::BeginMenu(_localizationDict->Get("File"_sid).c_str()))
        {
            ImGui::Separator();

            ComposeMenuFileQuit();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuView()
    {
        if (ImGui::BeginMenu(_localizationDict->Get("View"_sid).c_str()))
        {
            ComposeMenuViewFullscreen();
            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuLanguage()
    {
        if (ImGui::BeginMenu(_localizationDict->Get("Language"_sid).c_str()))
        {
            if (ImGui::MenuItem(_localizationManager->Translation(SidTrDomainEngine, SidTrLocaleEnName).c_str()))
            {
                _localizationManager->SetLocale(LocaleEnUTF8Keyword);
            }

            if (ImGui::MenuItem(_localizationManager->Translation(SidTrDomainEngine, SidTrLocaleRuName).c_str()))
            {
                _localizationManager->SetLocale(LocaleRuUTF8Keyword);
            }

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFileQuit()
    {
        if (ImGui::MenuItem(_localizationDict->Get("Quit"_sid).c_str(), Shortcuts::Quit.text))
        {
            _popups.quit = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuViewFullscreen()
    {
        const auto screenMode = _window->GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;
        if (ImGui::MenuItem(_localizationDict->Get("Fullscreen"_sid).c_str(), Shortcuts::Fullscreen.text, &isFullscreen))
        {
            _window->SetScreenMode(isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposePopups()
    {
        if (_popups.quit)
        {
            PopupQuit();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::PopupQuit()
    {
        if (false /*future logic*/)
        {
        }
        else
        {
            _window->SetShouldClose(true);
            _popups.quit = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::SwitchFullscreen()
    {
        const auto isFullscreen = _window->GetScreenMode() == Window::WindowedFullscreenMode;
        _window->SetScreenMode(!isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeStatusBar()
    {
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnWindowCloseEvent(WindowCloseEvent&)
    {
        _popups.quit = true;
        _window->SetShouldClose(false);
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnKeyPressEvent(KeyPressEvent& event)
    {
        const auto keyCode = event.GetKeyCode();
        const auto mods = event.GetMods();

        if (event.IsRepeat())
        {
            return true;
        }

        if (Shortcuts::Quit.Accept(mods, keyCode))
        {
            _popups.quit = true;
        }
        else if (Shortcuts::Fullscreen.Accept(mods, keyCode))
        {
            SwitchFullscreen();
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::FillDictionary()
    {
        _localizationDict->Add("File"_sid, Translator::Translate(KMP_TR_DOMAIN_EDITOR, "File"));
        _localizationDict->Add("View"_sid, Translator::Translate(KMP_TR_DOMAIN_EDITOR, "View"));
        _localizationDict->Add("Quit"_sid, Translator::Translate(KMP_TR_DOMAIN_EDITOR, "Quit"));
        _localizationDict->Add("Fullscreen"_sid, Translator::Translate(KMP_TR_DOMAIN_EDITOR, "Fullscreen"));
        _localizationDict->Add("Language"_sid, Translator::Translate(KMP_TR_DOMAIN_EDITOR, "Language"));
    }
    //--------------------------------------------------------------------------
}