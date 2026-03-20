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
#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            static PFN_vkCreateDebugUtilsMessengerEXT CreateDebugUtilsMessengerEXT;
            static PFN_vkDestroyDebugUtilsMessengerEXT DestroyDebugUtilsMessengerEXT;
#endif
            static PFN_vkCmdSetRasterizationSamplesEXT CmdSetRasterizationSamplesEXT;

            KMP_NODISCARD static bool LoadExtensionFunctions(VkInstance instance);
        };
        //--------------------------------------------------------------------------
    }
}