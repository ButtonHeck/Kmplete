#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"

namespace Kmplete
{
    class EditorUICompositor
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUICompositor)

        explicit EditorUICompositor(const Ptr<Window> window);

        void ComposeMainArea();
        void ComposeStatusBar();

        KMP_NODISCARD bool OnWindowCloseEvent(WindowCloseEvent& event);
        KMP_NODISCARD bool OnKeyPressEvent(KeyPressEvent& event);

    private:
        void ComposeDefaultLayout();

        void ComposeMenu();
        void ComposeMenuFile();
        void ComposeMenuView();
        void ComposeMenuFileQuit();
        void ComposeMenuViewFullscreen();

        void ComposePopups();
        void PopupQuit();

        void SwitchFullscreen();

    private:
        struct UIPopupsState
        {
            bool quit = false;
        };

    private:
        const Ptr<Window> _window;
        UIPopupsState _popups;
    };
    //--------------------------------------------------------------------------
}