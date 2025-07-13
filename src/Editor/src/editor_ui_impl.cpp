#include "editor_ui_impl.h"
#include "editor_ui_impl_opengl_glfw.h"
#include "Kmplete/Profile/profiler.h"

namespace Kmplete
{
    EditorUIImpl* EditorUIImpl::CreateImpl(Window& mainWindow, GraphicsBackendType graphicsBackendType)
    {
        KMP_PROFILE_FUNCTION();

#if defined (KMP_WINDOW_BACKEND_GLFW)
        if (graphicsBackendType == GraphicsBackendType::OpenGL)
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
