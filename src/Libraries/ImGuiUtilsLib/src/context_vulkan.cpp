#include "Kmplete/ImGui/context_vulkan.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ContextVulkan::ContextVulkan(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, ImGui_ImplVulkan_InitInfo initInfo)
            : Context(window, graphicsBackendType, dockingEnabled, viewportsEnabled)
            , initInfo(initInfo)
        {}
        //--------------------------------------------------------------------------
    }
}