#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"

namespace Kmplete
{
    class LocalizationManager;
    class SystemMetricsManager;

    class EditorUICompositor
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUICompositor)

        EditorUICompositor(Window& mainWindow, LocalizationManager& localizationManager, const SystemMetricsManager& systemMetricsManager);

        void ComposeMainArea();
        void ComposeStatusBar();

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event);
        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event);

    private:
        void ComposeDefaultLayout();

        void ComposeMenu();
        void ComposeMenuFile();
        void ComposeMenuView();
        void ComposeMenuLanguage();
        void ComposeMenuFileQuit();
        void ComposeMenuViewFullscreen();

        void ComposePopups();
        void PopupQuit();

        void SwitchFullscreen();

        void FillDictionary();

    private:
        struct UIPopupsState
        {
            bool quit = false;
        };

    private:
        Window& _mainWindow;
        LocalizationManager& _localizationManager;
        const SystemMetricsManager& _systemMetricsManager;
        UIPopupsState _popups;
    };
    //--------------------------------------------------------------------------
}