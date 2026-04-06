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
            static PFN_vkCmdSetDepthClampEnableEXT CmdSetDepthClampEnableEXT;
            static PFN_vkCmdSetDepthClipEnableEXT CmdSetDepthClipEnableEXT;
            static PFN_vkCmdSetDepthClampRangeEXT CmdSetDepthClampRangeEXT;
            static PFN_vkCmdSetLineStippleEnableEXT CmdSetLineStippleEnableEXT;
            static PFN_vkCmdSetLineRasterizationModeEXT CmdSetLineRasterizationModeEXT;

            KMP_NODISCARD static bool LoadExtensionFunctions(VkInstance instance);
        };
        //--------------------------------------------------------------------------
    }
}