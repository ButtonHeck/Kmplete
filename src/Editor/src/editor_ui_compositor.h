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

        bool OnWindowCloseEvent(WindowCloseEvent& event);
        bool OnKeyPressEvent(KeyPressEvent& event);

    private:
        const Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}