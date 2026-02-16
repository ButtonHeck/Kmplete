#pragma once

#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/ImGui/context.h"

#include <backends/imgui_impl_vulkan.h>


namespace Kmplete
{
    namespace ImGuiUtils
    {
        //TODO: comments
        struct ContextVulkan : public Context
        {
            ContextVulkan(void* window, String graphicsBackendType, bool dockingEnabled, bool viewportsEnabled, ImGui_ImplVulkan_InitInfo initInfo);

            ImGui_ImplVulkan_InitInfo initInfo;
        };
        //--------------------------------------------------------------------------
    }
}