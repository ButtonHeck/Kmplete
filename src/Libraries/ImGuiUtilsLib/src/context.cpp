#include "Kmplete/ImGui/context.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        Context::Context(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled)
            : window(window)
            , graphicsBackendType(graphicsBackendType)
            , dockingEnabled(dockingEnabled)
            , viewportsEnabled(viewportsEnabled)
        {}
        //--------------------------------------------------------------------------
    }
}