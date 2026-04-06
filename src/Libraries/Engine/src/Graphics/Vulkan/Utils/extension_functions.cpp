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
        PFN_vkCmdSetDepthClampEnableEXT VulkanCommands::CmdSetDepthClampEnableEXT = nullptr;
        PFN_vkCmdSetDepthClipEnableEXT VulkanCommands::CmdSetDepthClipEnableEXT = nullptr;
        PFN_vkCmdSetDepthClampRangeEXT VulkanCommands::CmdSetDepthClampRangeEXT = nullptr;
        PFN_vkCmdSetLineStippleEnableEXT VulkanCommands::CmdSetLineStippleEnableEXT = nullptr;
        PFN_vkCmdSetLineRasterizationModeEXT VulkanCommands::CmdSetLineRasterizationModeEXT = nullptr;


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

            CmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDepthClampEnableEXT");
            if (CmdSetDepthClampEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetDepthClampEnableEXT function");
                return false;
            }

            CmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDepthClipEnableEXT");
            if (CmdSetDepthClipEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetDepthClipEnableEXT function");
                return false;
            }

            CmdSetDepthClampRangeEXT = (PFN_vkCmdSetDepthClampRangeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDepthClampRangeEXT");
            if (CmdSetDepthClampRangeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetDepthClampRangeEXT function");
                return false;
            }

            CmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetLineStippleEnableEXT");
            if (CmdSetLineStippleEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetLineStippleEnableEXT function");
                return false;
            }

            CmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetLineRasterizationModeEXT");
            if (CmdSetLineRasterizationModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetLineRasterizationModeEXT function");
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------
    }
}