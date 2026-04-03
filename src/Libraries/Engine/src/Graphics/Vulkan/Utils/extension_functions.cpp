#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        PFN_vkCreateDebugUtilsMessengerEXT VulkanCommands::CreateDebugUtilsMessengerEXT = nullptr;
        PFN_vkDestroyDebugUtilsMessengerEXT VulkanCommands::DestroyDebugUtilsMessengerEXT = nullptr;
#endif
        PFN_vkCmdSetRasterizationSamplesEXT VulkanCommands::CmdSetRasterizationSamplesEXT = nullptr;


        bool VulkanCommands::LoadExtensionFunctions(VkInstance instance) KMP_PROFILING(ProfileLevelAlways)
        {
#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (CreateDebugUtilsMessengerEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCreateDebugUtilsMessengerEXT function");
                return false;
            }

            DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (DestroyDebugUtilsMessengerEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkDestroyDebugUtilsMessengerEXT function");
                return false;
            }
#endif

            CmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)vkGetInstanceProcAddr(instance, "vkCmdSetRasterizationSamplesEXT");
            if (CmdSetRasterizationSamplesEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetRasterizationSamplesEXT function");
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------
    }
}