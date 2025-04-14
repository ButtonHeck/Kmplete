#include "editor_ui_impl.h"
#include "editor_ui_impl_opengl_glfw.h"

namespace Kmplete
{
    EditorUIImpl* EditorUIImpl::CreateImpl(Window& mainWindow, GraphicsBackend::BackendType graphicsBackendType)
    {
#if defined (KMP_WINDOW_BACKEND_GLFW)
        if (graphicsBackendType == GraphicsBackend::BackendType::OpenGL)
        {
            return new EditorUIImplOpenglGLFW(mainWindow);
        }
#else
    #error "No suitable window/graphics backends are provided!"
#endif
        return nullptr;
    }
    //--------------------------------------------------------------------------

    EditorUIImpl::EditorUIImpl(Window& mainWindow)
        : _mainWindow(mainWindow)
    {}
    //--------------------------------------------------------------------------
}
