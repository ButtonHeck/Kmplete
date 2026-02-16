#include "Kmplete/ImGui/context_opengl.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ContextOpenGL::ContextOpenGL(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled)
            : Context(window, graphicsBackendType, dockingEnabled, viewportsEnabled)
        {}
        //--------------------------------------------------------------------------
    }
}