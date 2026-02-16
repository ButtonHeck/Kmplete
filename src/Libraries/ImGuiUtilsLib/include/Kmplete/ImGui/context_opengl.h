#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/ImGui/context.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //TODO: comments
        struct ContextOpenGL : public Context
        {
            ContextOpenGL(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled);
        };
        //--------------------------------------------------------------------------
    }
}