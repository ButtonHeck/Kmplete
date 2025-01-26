#pragma once

#include "editor_ui_impl.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/kmplete_api.h"

namespace Kmplete
{
    class EditorUI
    {
    public:
        KMP_DISABLE_COPY_MOVE(EditorUI)

        explicit EditorUI(const Ptr<Window> window);
        ~EditorUI();

    private:
        void Initialize(const Ptr<Window> window);
        void Finalize();

    private:
        UPtr<EditorUIImpl> _uiImpl;
    };
    //--------------------------------------------------------------------------
}