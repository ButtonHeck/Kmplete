#pragma once

#include "editor_ui_impl.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"

namespace Kmplete
{
    class EditorUI
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUI)

        explicit EditorUI(const Ptr<Window> window);
        ~EditorUI();

        void LoopIteration();

        void NewFrame();
        void Stylize();
        void BeginApplicationArea();
        void BeginMainWorkingArea();
        void Compose();
        void EndMainWorkingArea();
        void ComposeStatusBar();
        void EndApplicationArea();
        void Render();
        void EndFrame();

        bool OnWindowCloseEvent(WindowCloseEvent& event);
        bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event);
        bool OnKeyPressEvent(KeyPressEvent& event);

        void SaveSettings(Settings& settings) const;
        void LoadSettings(Settings& settings);

    private:
        void Initialize(const Ptr<Window> window);
        void Finalize();

    private:
        UPtr<EditorUIImpl> _uiImpl;
    };
    //--------------------------------------------------------------------------
}