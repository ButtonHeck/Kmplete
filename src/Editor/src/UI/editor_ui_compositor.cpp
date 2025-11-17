#include "UI/editor_ui_compositor.h"
#include "UI/shortcuts.h"
#include "UI/ui_identifiers.h"
#include "Application/localization_base.h"

#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Assets/texture_asset_manager.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/ImGui/helper_functions.h"
#include "Kmplete/ImGui/scope_guards.h"

#include <imgui_internal.h> // for ImGui::DockBuilder api
#include <misc/cpp/imgui_stdlib.h> // for ImGui::InputText wrappers for std::string
#include <forkawesome-webfont.h>


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "EditorUICompositor";
    static constexpr auto MetricsFractionalStr = "MetricsFractional";


    EditorUICompositor::EditorUICompositor(Window& mainWindow, Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager, const SystemMetricsManager& systemMetricsManager)
        : KMP_PROFILE_CONSTRUCTOR_START_BASE_CLASS()
          _mainWindow(mainWindow)
        , _assetsManager(assetsManager)
        , _localizationManager(localizationManager)
        , _systemMetricsManager(systemMetricsManager)
        , _needCheckImguiIniFile(true)
    {
        _FillDictionary();
        _localizationManager.AddLocaleChangedCallback(KMP_BIND(EditorUICompositor::_FillDictionary));

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMainArea()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_needCheckImguiIniFile)
        {
            _needCheckImguiIniFile = false;
            if (!Filesystem::FilepathExists(Filesystem::GetCurrentFilepath().append(ImGui::GetIO().IniFilename)))
            {
                _ComposeDefaultLayout();
            }
        }

        _ComposeMenu();

        _ComposePopups();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeDefaultLayout()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenu()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (ImGui::BeginMenuBar())
        {
            _ComposeMenuLanguage();
            _ComposeMenuFile();
            _ComposeMenuView();

            ImGui::EndMenuBar();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenuLanguage()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        const auto dpiScale = _mainWindow.GetDPIScale();
        const auto iconSize = ImVec2(18 * dpiScale, 18 * dpiScale);

        static const ImTextureID languageIcons[] = {
            static_cast<ImTextureID>(_assetsManager.GetTextureAssetManager().GetAsset("_flag_usa"_sid).GetTexture().GetHandle()),
            static_cast<ImTextureID>(_assetsManager.GetTextureAssetManager().GetAsset("_flag_russian"_sid).GetTexture().GetHandle())
        };

        int languageIndex = 0;
        if (_localizationManager.GetLocale() == LocaleEnUTF8Keyword)
        {
            languageIndex = 0;
        }
        else if (_localizationManager.GetLocale() == LocaleRuUTF8Keyword)
        {
            languageIndex = 1;
        }

        ImGuiUtils::StyleColorGuard colorGuard({ {ImGuiCol_Button, ImColor(0, 0, 0, 0)}, {ImGuiCol_Border, ImColor(0, 0, 0, 0)} });
        if (ImGui::ImageButton(languageIcons[languageIndex], iconSize))
        {
            ImGui::OpenPopup(IdPopup_ChangeLanguage);
        }
        ImGuiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Change language"_sid).c_str());

        if (ImGui::BeginPopup(IdPopup_ChangeLanguage))
        {
            const auto EngButtonClicked = ImGui::ImageButton(languageIcons[0], iconSize);
            ImGuiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEngine, "English"_sid).c_str());

            const auto RusButtonClicked = ImGui::ImageButton(languageIcons[1], iconSize);
            ImGuiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEngine, "Russian"_sid).c_str());

            if (EngButtonClicked)
            {
                _localizationManager.SetLocale(LocaleEnUTF8Keyword);
            }
            else if (RusButtonClicked)
            {
                _localizationManager.SetLocale(LocaleRuUTF8Keyword);
            }

            ImGui::EndPopup();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenuFile()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (ImGui::BeginMenu(_localizationManager.Translation(SidTrDomainEditor, "File"_sid).c_str()))
        {
            ImGui::Separator();

            _ComposeMenuFileQuit();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenuView()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (ImGui::BeginMenu(_localizationManager.Translation(SidTrDomainEditor, "View"_sid).c_str()))
        {
            _ComposeMenuViewFullscreen();
            _ComposeMenuViewAlwaysOnTop();
            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenuFileQuit()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEditor, "Quit"_sid).c_str(), Shortcuts::Quit.text))
        {
            _popups.quit = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenuViewFullscreen()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        auto isWindowedFullscreen = _mainWindow.IsWindowedFullscreen();
        if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEditor, "Fullscreen"_sid).c_str(), Shortcuts::Fullscreen.text, &isWindowedFullscreen))
        {
            _mainWindow.SetScreenMode(isWindowedFullscreen ? Window::ScreenMode::WindowedFullscreen : Window::ScreenMode::Windowed);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposeMenuViewAlwaysOnTop()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        const auto isWindowed = _mainWindow.IsWindowed();
        auto isAlwaysOnTop = _mainWindow.IsAlwaysOnTop();
        if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEditor, "Always on top"_sid).c_str(), Shortcuts::AlwaysOnTop.text, &isAlwaysOnTop, isWindowed))
        {
            _mainWindow.SetAlwaysOnTop(isAlwaysOnTop);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_ComposePopups()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_popups.quit)
        {
            _PopupQuit();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_PopupQuit()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (false /*future logic*/)
        {
        }
        else
        {
            _mainWindow.SetShouldClose(true);
            _popups.quit = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_SwitchFullscreen()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        _mainWindow.SetScreenMode(!_mainWindow.IsWindowedFullscreen() ? Window::ScreenMode::WindowedFullscreen : Window::ScreenMode::Windowed);
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_SwitchAlwaysOnTop()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        _mainWindow.SetAlwaysOnTop(!_mainWindow.IsAlwaysOnTop());
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeStatusBar(Timer& metricsTimer)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        const auto dpiScale = _mainWindow.GetDPIScale();
        const auto buttonSize = ImVec2(24 * dpiScale, 24 * dpiScale);
        ImGui::SetCursorPosX(8.0f);
        ImGui::SetCursorPosY(2.0f * dpiScale);

        if (ImGui::Button(ICON_FK_CLOCK_O, buttonSize))
        {
            ImGui::OpenPopup(IdPopup_StatusBarSettings);
        }
        ImGuiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Metrics update period (ms)"_sid).c_str());

        if (ImGui::BeginPopup(IdPopup_StatusBarSettings))
        {
            const char* intervals[] = {"500", "1000", "2000"};
            for (auto intervalIndex = 0; intervalIndex < 3; intervalIndex++)
            {
                if (ImGui::Selectable(intervals[intervalIndex]))
                {
                    metricsTimer.SetTimeout(std::stoi(intervals[intervalIndex]));
                }
            }

            ImGui::EndPopup();
        }

        ImGui::SameLine();

        {
            ImGuiUtils::StyleColorGuard colorGuard({ {ImGuiCol_Border, _state.metricsFractional ? ImVec4(1, 1, 1, 1) : ImGui::GetStyleColorVec4(ImGuiCol_Border)} });
            if (ImGui::Button(ICON_FK_PERCENT, buttonSize))
            {
                _state.metricsFractional = !_state.metricsFractional;
            }
        }
        ImGuiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Show fractional"_sid).c_str());

        ImGui::SameLine();

        const auto& currentMetrics = _systemMetricsManager.GetMetrics();
        std::ostringstream oss;
        oss.setf(std::ios::fixed);
        oss.precision(_state.metricsFractional ? 2 : 0);
        const auto metricsString = Utils::ToSStream(oss,
            "CPU: ", currentMetrics.cpuUsagePercent, "%, ",
            "PMem: ", currentMetrics.physicalMemoryUsedMib, "MiB, ",
            "VMem: ", currentMetrics.virtualMemoryUsedMib, "MiB, ",
            "Stack usage: ", currentMetrics.currentThreadStackUsed, "KiB / ",
            currentMetrics.currentThreadStackTotal, "KiB (",
            currentMetrics.currentThreadStackUsagePercent, "%)").str();

        ImGui::TextUnformatted(metricsString.c_str());
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnWindowCloseEvent(WindowCloseEvent&)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        _popups.quit = true;
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnKeyPressEvent(KeyPressEvent& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

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
            _SwitchFullscreen();
        }
        else if (Shortcuts::AlwaysOnTop.Accept(mods, keyCode))
        {
            _SwitchAlwaysOnTop();
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveBool(MetricsFractionalStr, _state.metricsFractional);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartLoadObject(SettingsEntryName);
        _state.metricsFractional = settings.GetBool(MetricsFractionalStr, true);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::_FillDictionary()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "File");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "View");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Quit");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Fullscreen");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Metrics update period (ms)");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Show fractional");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Change language");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Always on top");
    }
    //--------------------------------------------------------------------------
}