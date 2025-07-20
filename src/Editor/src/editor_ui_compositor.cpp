#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "shortcuts.h"
#include "ui_identifiers.h"
#include "localization_base.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Localization/localization_manager.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Graphics/texture_manager.h"

#include <imgui.h>
#include <imgui_internal.h> // for ImGui::DockBuilder api
#include <misc/cpp/imgui_stdlib.h> // for ImGui::InputText wrappers for std::string
#include <forkawesome-webfont.h>

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "EditorUICompositor";
    constexpr static auto MetricsFractionalStr = "MetricsFractional";

    EditorUICompositor::EditorUICompositor(Window& mainWindow, GraphicsBackend& graphicsBackend, LocalizationManager& localizationManager, const SystemMetricsManager& systemMetricsManager)
        :
#if defined(KMP_PROFILE)
        _constructorProfilerTimer(CreateUPtr<ProfilerTimer>("EditorUICompositor::EditorUICompositor(Window&, GraphicsBackend&, LocalizationManager&, const SystemMetricsManager&)")),
#endif
        _mainWindow(mainWindow)
        , _graphicsBackend(graphicsBackend)
        , _localizationManager(localizationManager)
        , _systemMetricsManager(systemMetricsManager)
        , _needCheckImguiIniFile(true)
    {
        FillDictionary();
        _localizationManager.AddLocaleChangedCallback(KMP_BIND(EditorUICompositor::FillDictionary));

#if defined(KMP_PROFILE)
        _constructorProfilerTimer.reset(nullptr);
#endif
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMainArea()
    {
        KMP_PROFILE_FUNCTION();

        if (_needCheckImguiIniFile)
        {
            _needCheckImguiIniFile = false;
            if (!Filesystem::PathExists(Filesystem::GetCurrentPath().append(ImGui::GetIO().IniFilename)))
            {
                ComposeDefaultLayout();
            }
        }

        ComposeMenu();

        ComposePopups();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeDefaultLayout()
    {
        KMP_PROFILE_FUNCTION();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenu()
    {
        KMP_PROFILE_FUNCTION();

        if (ImGui::BeginMenuBar())
        {
            ComposeMenuLanguage();
            ComposeMenuFile();
            ComposeMenuView();

            ImGui::EndMenuBar();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuLanguage()
    {
        KMP_PROFILE_FUNCTION();

        const auto dpiScale = _mainWindow.GetDPIScale();
        const auto iconSize = ImVec2(18 * dpiScale, 18 * dpiScale);

        static const ImTextureID languageIcons[] = {
            static_cast<ImTextureID>(_graphicsBackend.GetTextureManager().GetTexture("_flag_usa"_sid).GetHandle()),
            static_cast<ImTextureID>(_graphicsBackend.GetTextureManager().GetTexture("_flag_russian"_sid).GetHandle())
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

        UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_Button, ImColor(0, 0, 0, 0)}, {ImGuiCol_Border, ImColor(0, 0, 0, 0)} });
        if (ImGui::ImageButton(languageIcons[languageIndex], iconSize))
        {
            ImGui::OpenPopup(IdPopup_ChangeLanguage);
        }
        UiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Change language"_sid).c_str());

        if (ImGui::BeginPopup(IdPopup_ChangeLanguage))
        {
            const auto EngButtonClicked = ImGui::ImageButton(languageIcons[0], iconSize);
            UiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEngine, "English"_sid).c_str());

            const auto RusButtonClicked = ImGui::ImageButton(languageIcons[1], iconSize);
            UiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEngine, "Russian"_sid).c_str());

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

    void EditorUICompositor::ComposeMenuFile()
    {
        KMP_PROFILE_FUNCTION();

        if (ImGui::BeginMenu(_localizationManager.Translation(SidTrDomainEditor, "File"_sid).c_str()))
        {
            ImGui::Separator();

            ComposeMenuFileQuit();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuView()
    {
        KMP_PROFILE_FUNCTION();

        if (ImGui::BeginMenu(_localizationManager.Translation(SidTrDomainEditor, "View"_sid).c_str()))
        {
            ComposeMenuViewFullscreen();
            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFileQuit()
    {
        KMP_PROFILE_FUNCTION();

        if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEditor, "Quit"_sid).c_str(), Shortcuts::Quit.text))
        {
            _popups.quit = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuViewFullscreen()
    {
        KMP_PROFILE_FUNCTION();

        const auto screenMode = _mainWindow.GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;
        if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEditor, "Fullscreen"_sid).c_str(), Shortcuts::Fullscreen.text, &isFullscreen))
        {
            _mainWindow.SetScreenMode(isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposePopups()
    {
        KMP_PROFILE_FUNCTION();

        if (_popups.quit)
        {
            PopupQuit();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::PopupQuit()
    {
        KMP_PROFILE_FUNCTION();

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

    void EditorUICompositor::SwitchFullscreen()
    {
        KMP_PROFILE_FUNCTION();

        const auto isFullscreen = _mainWindow.GetScreenMode() == Window::WindowedFullscreenMode;
        _mainWindow.SetScreenMode(!isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeStatusBar(Timer& metricsTimer)
    {
        KMP_PROFILE_FUNCTION();

        const auto dpiScale = _mainWindow.GetDPIScale();
        const auto buttonSize = ImVec2(24 * dpiScale, 24 * dpiScale);
        ImGui::SetCursorPosX(8.0f);
        ImGui::SetCursorPosY(2.0f * dpiScale);

        if (ImGui::Button(ICON_FK_CLOCK_O, buttonSize))
        {
            ImGui::OpenPopup(IdPopup_StatusBarSettings);
        }
        UiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Metrics update period (ms)"_sid).c_str());

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
            UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_Border, _state.metricsFractional ? ImVec4(1, 1, 1, 1) : ImGui::GetStyleColorVec4(ImGuiCol_Border)} });
            if (ImGui::Button(ICON_FK_PERCENT, buttonSize))
            {
                _state.metricsFractional = !_state.metricsFractional;
            }
        }
        UiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Show fractional"_sid).c_str());

        ImGui::SameLine();

        const auto& currentMetrics = _systemMetricsManager.GetMetrics();
        std::ostringstream oss;
        oss.setf(std::ios::fixed);
        oss.precision(_state.metricsFractional ? 2 : 0);
        const auto metricsString = Utils::ToSStream(oss,
            "CPU: ", currentMetrics.cpuUsagePercent, "%, ",
            "PMem: ", currentMetrics.physicalMemoryUsedMib, "MiB, ",
            "VMem: ", currentMetrics.virtualMemoryUsedMib, "MiB").str();

        ImGui::TextUnformatted(metricsString.c_str());
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnWindowCloseEvent(WindowCloseEvent&)
    {
        KMP_PROFILE_FUNCTION();

        _popups.quit = true;
        _mainWindow.SetShouldClose(false);
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnKeyPressEvent(KeyPressEvent& event)
    {
        KMP_PROFILE_FUNCTION();

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

    void EditorUICompositor::SaveSettings(Settings& settings) const
    {
        KMP_PROFILE_FUNCTION();

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveBool(MetricsFractionalStr, _state.metricsFractional);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::LoadSettings(Settings& settings)
    {
        KMP_PROFILE_FUNCTION();

        settings.StartLoadObject(SettingsEntryName);
        _state.metricsFractional = settings.GetBool(MetricsFractionalStr, true);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::FillDictionary()
    {
        KMP_PROFILE_FUNCTION();

        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "File");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "View");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Quit");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Fullscreen");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Metrics update period (ms)");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Show fractional");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Change language");
    }
    //--------------------------------------------------------------------------
}