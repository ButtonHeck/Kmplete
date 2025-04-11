#include "editor_ui_impl.h"
#include "editor_ui_impl_opengl_glfw.h"
#include "Kmplete/Graphics/graphics_backend.h"

namespace Kmplete
{
    EditorUIImpl* EditorUIImpl::CreateImpl(const Ptr<Window> window, const std::string& graphicsApiName)
    {
#if defined (KMP_WINDOW_BACKEND_GLFW)
        if (graphicsApiName == GraphicsBackend::OpenGLStr)
        {
            return new EditorUIImplOpenglGLFW(window);
        }
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
