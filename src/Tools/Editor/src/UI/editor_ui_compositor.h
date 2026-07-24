#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/ImGui/implementation.h"
#include "Kmplete/Profile/profiler_fwd.h"


namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;
    class SettingsDocument;

    namespace Assets
    {
        class AssetsManager;
    }

    namespace Time
    {
        class Timer;
    }

    namespace Input
    {
        class InputManager;
    }


    class EditorUICompositor
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(EditorUICompositor)

    public:
        EditorUICompositor(Window& mainWindow, Assets::AssetsManager& assetsManager, LocalizationManager& localizationManager, 
                           const SystemMetricsManager& systemMetricsManager, Input::InputManager& inputManager);
        ~EditorUICompositor() = default;

        void ComposeMainArea(const ImGuiUtils::ImGuiImplementation& imguiImpl);
        void ComposeStatusBar(Time::Timer& metricsTimer, const ImGuiUtils::ImGuiImplementation& imguiImpl);

        KMP_NODISCARD bool OnWindowCloseEvent(Events::WindowCloseEvent& event);

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        void _ComposeDefaultLayout();

        void _ComposeMenu(const ImGuiUtils::ImGuiImplementation& imguiImpl);
        void _ComposeMenuLanguage(const ImGuiUtils::ImGuiImplementation& imguiImpl);
        void _ComposeMenuFile();
        void _ComposeMenuView();
        void _ComposeMenuFileQuit();
        void _ComposeMenuViewFullscreen();
        void _ComposeMenuViewAlwaysOnTop();

        void _ComposePopups();
        void _PopupQuit();

        void _SwitchFullscreen();
        void _SwitchAlwaysOnTop();

        void _FillDictionary();

    private:
        struct _UIComponentsState
        {
            bool metricsFractional = true;
        };

        struct _UIPopupsState
        {
            bool quit = false;
        };

    private:
        Window& _mainWindow;
        Assets::AssetsManager& _assetsManager;
        LocalizationManager& _localizationManager;
        const SystemMetricsManager& _systemMetricsManager;
        _UIComponentsState _state;
        _UIPopupsState _popups;
        bool _needCheckImguiIniFile;
    };
    //--------------------------------------------------------------------------
}