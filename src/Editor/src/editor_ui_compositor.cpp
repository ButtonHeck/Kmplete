#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "shortcuts.h"
#include "localization_base.h"
#include "Kmplete/Core/filesystem.h"
#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/timer.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Localization/localization_manager.h"

#include "Kmplete/Graphics/OpenGL/opengl_texture.h" // TODO remove after refactoring

#include <imgui.h>
#include <imgui_internal.h> // for ImGui::DockBuilder api
#include <misc/cpp/imgui_stdlib.h> // for ImGui::InputText wrappers for std::string
#include <forkawesome-webfont.h>

namespace Kmplete
{
    constexpr static auto EditorUICompositorSettingsEntryName = "EditorUICompositor";
    constexpr static auto EditorUICompositorMetricsFractionalStr = "MetricsFractional";

    EditorUICompositor::EditorUICompositor(Window& mainWindow, LocalizationManager& localizationManager, const SystemMetricsManager& systemMetricsManager)
        : _mainWindow(mainWindow)
        , _localizationManager(localizationManager)
        , _systemMetricsManager(systemMetricsManager)
    {
        FillDictionary();
        _localizationManager.AddLocaleChangedCallback(KMP_BIND(EditorUICompositor::FillDictionary));
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
        static const OpenGLTexture testTexture = OpenGLTexture(Utils::Concatenate(KMP_ICONS_FOLDER, "/flag_russia_128.png"), ImageChannels::RGB, false); //TODO remove test
        if (ImGui::BeginMenuBar())
        {
            ImGui::ImageButton(static_cast<ImTextureID>(testTexture.GetHandle()), ImVec2{24, 24});
            ComposeMenuFile();
            ComposeMenuView();
            ComposeMenuLanguage();

            ImGui::EndMenuBar();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFile()
    {
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
        if (ImGui::BeginMenu(_localizationManager.Translation(SidTrDomainEditor, "View"_sid).c_str()))
        {
            ComposeMenuViewFullscreen();
            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuLanguage()
    {
        if (ImGui::BeginMenu(_localizationManager.Translation(SidTrDomainEditor, "Language"_sid).c_str()))
        {
            if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEngine, SidTrLocaleEnName).c_str()))
            {
                _localizationManager.SetLocale(LocaleEnUTF8Keyword);
            }

            if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEngine, SidTrLocaleRuName).c_str()))
            {
                _localizationManager.SetLocale(LocaleRuUTF8Keyword);
            }

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuFileQuit()
    {
        if (ImGui::MenuItem(_localizationManager.Translation(SidTrDomainEditor, "Quit"_sid).c_str(), Shortcuts::Quit.text))
        {
            _popups.quit = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMenuViewFullscreen()
    {
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
            _mainWindow.SetShouldClose(true);
            _popups.quit = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::SwitchFullscreen()
    {
        const auto isFullscreen = _mainWindow.GetScreenMode() == Window::WindowedFullscreenMode;
        _mainWindow.SetScreenMode(!isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeStatusBar(Timer& metricsTimer)
    {
        ImGui::SetCursorPosX(8.0f);

        if (ImGui::Button(ICON_FK_CLOCK_O))
        {
            ImGui::OpenPopup("StatusBarSettingsPopup");
        }
        UiUtils::SetItemTooltip(_localizationManager.Translation(SidTrDomainEditor, "Metrics update period (ms)"_sid).c_str());

        if (ImGui::BeginPopup("StatusBarSettingsPopup"))
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
            if (ImGui::Button(ICON_FK_PERCENT))
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
        _popups.quit = true;
        _mainWindow.SetShouldClose(false);
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

    void EditorUICompositor::SaveSettings(Settings& settings) const
    {
        settings.StartSaveObject(EditorUICompositorSettingsEntryName);
        settings.SaveBool(EditorUICompositorMetricsFractionalStr, _state.metricsFractional);
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::LoadSettings(Settings& settings)
    {
        settings.StartLoadObject(EditorUICompositorSettingsEntryName);
        _state.metricsFractional = settings.GetBool(EditorUICompositorMetricsFractionalStr, true);
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::FillDictionary()
    {
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "File");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "View");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Quit");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Fullscreen");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Language");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Metrics update period (ms)");
        _localizationManager.Translate(KMP_TR_DOMAIN_EDITOR, "Show fractional");
    }
    //--------------------------------------------------------------------------
}