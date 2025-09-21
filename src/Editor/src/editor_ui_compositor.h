#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;
    class Timer;
    class SettingsDocument;
    class GraphicsBackend;

    class EditorUICompositor
    {
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(EditorUICompositor)

        EditorUICompositor(Window& mainWindow, GraphicsBackend& graphicsBackend, LocalizationManager& localizationManager, const SystemMetricsManager& systemMetricsManager);

        void ComposeMainArea();
        void ComposeStatusBar(Timer& metricsTimer);

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event);
        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event);

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        void ComposeDefaultLayout();

        void ComposeMenu();
        void ComposeMenuLanguage();
        void ComposeMenuFile();
        void ComposeMenuView();
        void ComposeMenuFileQuit();
        void ComposeMenuViewFullscreen();
        void ComposeMenuViewAlwaysOnTop();

        void ComposePopups();
        void PopupQuit();

        void SwitchFullscreen();
        void SwitchAlwaysOnTop();

        void FillDictionary();

    private:
        struct UIComponentsState
        {
            bool metricsFractional = true;
        };

        struct UIPopupsState
        {
            bool quit = false;
        };

    private:
        Window& _mainWindow;
        GraphicsBackend& _graphicsBackend;
        LocalizationManager& _localizationManager;
        const SystemMetricsManager& _systemMetricsManager;
        UIComponentsState _state;
        UIPopupsState _popups;
        bool _needCheckImguiIniFile;
    };
    //--------------------------------------------------------------------------
}