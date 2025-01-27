#include "editor_ui_compositor.h"

namespace Kmplete
{
    EditorUICompositor::EditorUICompositor(const Ptr<Window> window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeMainArea()
    {
    }
    //--------------------------------------------------------------------------

    void EditorUICompositor::ComposeStatusBar()
    {
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnWindowCloseEvent(WindowCloseEvent&)
    {
        _window->SetShouldClose(true);
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUICompositor::OnKeyPressEvent(KeyPressEvent&)
    {
        return true;
    }
    //--------------------------------------------------------------------------
}