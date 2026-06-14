#include "Kmplete/ImGui/context_vulkan.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace ImGuiUtils
    {
        ContextVulkan::ContextVulkan(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, float baseScale, ImGui_ImplVulkan_InitInfo initInfo)
            : Context(window, graphicsBackendType, dockingEnabled, viewportsEnabled, baseScale)
              KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
            , initInfo(initInfo)
        {
            KMP_PROFILE_CONSTRUCTOR_END()
        }
        //--------------------------------------------------------------------------
    }
}