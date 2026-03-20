#pragma once

#include "Kmplete/Base/kmplete_api.h"

#include <vulkan/vulkan.h>


namespace Kmplete
{
    namespace Graphics
    {
        //TODO: comments
        struct VulkanCommands
        {
            static PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
            static PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
            static PFN_vkCmdSetRasterizationSamplesEXT CmdSetRasterizationSamplesEXT;

            KMP_NODISCARD KMP_API static bool LoadExtensionFunctions(VkInstance instance);
        };
        //--------------------------------------------------------------------------
    }
}