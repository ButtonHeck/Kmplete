#include "editor_ui_impl.h"
#include "editor_ui_impl_opengl_glfw.h"

namespace Kmplete
{
    EditorUIImpl* EditorUIImpl::CreateImpl(const Ptr<Window> window)
    {
#if defined (KMP_WINDOW_BACKEND_GLFW)
        return new EditorUIImplOpenglGLFW(window);
#else
    #error "No suitable window/graphics backends are provided!"
#endif
        return nullptr;
    }
    //--------------------------------------------------------------------------

    EditorUIImpl::EditorUIImpl(const Ptr<Window> window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------
}
