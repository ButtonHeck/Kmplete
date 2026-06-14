#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/ImGui/context.h"
#include "Kmplete/Profile/profiler_fwd.h"

#include <backends/imgui_impl_vulkan.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //TODO: comments
        struct ContextVulkan : public Context
        {
            KMP_PROFILE_CONSTRUCTOR_DECLARE()

        public:
            ContextVulkan(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, float baseScale, ImGui_ImplVulkan_InitInfo initInfo);

            ImGui_ImplVulkan_InitInfo initInfo;
        };
        //--------------------------------------------------------------------------
    }
}