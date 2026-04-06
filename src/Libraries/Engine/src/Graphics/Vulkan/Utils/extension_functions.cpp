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
        PFN_vkCmdSetDiscardRectangleEnableEXT VulkanCommands::CmdSetDiscardRectangleEnableEXT = nullptr;
        PFN_vkCmdSetDiscardRectangleEXT VulkanCommands::CmdSetDiscardRectangleEXT = nullptr;
        PFN_vkCmdSetDiscardRectangleModeEXT VulkanCommands::CmdSetDiscardRectangleModeEXT = nullptr;
        PFN_vkCmdSetSampleLocationsEnableEXT VulkanCommands::CmdSetSampleLocationsEnableEXT = nullptr;
        PFN_vkCmdSetSampleLocationsEXT VulkanCommands::CmdSetSampleLocationsEXT = nullptr;
        PFN_vkCmdSetSampleMaskEXT VulkanCommands::CmdSetSampleMaskEXT = nullptr;
        PFN_vkCmdSetFragmentShadingRateKHR VulkanCommands::CmdSetFragmentShadingRateKHR = nullptr;
        PFN_vkCmdSetColorWriteEnableEXT VulkanCommands::CmdSetColorWriteEnableEXT = nullptr;
        PFN_vkCmdSetColorBlendEnableEXT VulkanCommands::CmdSetColorBlendEnableEXT = nullptr;
        PFN_vkCmdSetColorBlendEquationEXT VulkanCommands::CmdSetColorBlendEquationEXT = nullptr;
        PFN_vkCmdSetPolygonModeEXT VulkanCommands::CmdSetPolygonModeEXT = nullptr;


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

            CmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDiscardRectangleEnableEXT");
            if (CmdSetDiscardRectangleEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetDiscardRectangleEnableEXT function");
                return false;
            }

            CmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDiscardRectangleEXT");
            if (CmdSetDiscardRectangleEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetDiscardRectangleEXT function");
                return false;
            }

            CmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDiscardRectangleModeEXT");
            if (CmdSetDiscardRectangleModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetDiscardRectangleModeEXT function");
                return false;
            }

            CmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetSampleLocationsEnableEXT");
            if (CmdSetSampleLocationsEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetSampleLocationsEnableEXT function");
                return false;
            }

            CmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)vkGetInstanceProcAddr(instance, "vkCmdSetSampleLocationsEXT");
            if (CmdSetSampleLocationsEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetSampleLocationsEXT function");
                return false;
            }

            CmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT)vkGetInstanceProcAddr(instance, "vkCmdSetSampleMaskEXT");
            if (CmdSetSampleMaskEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetSampleMaskEXT function");
                return false;
            }

            CmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR)vkGetInstanceProcAddr(instance, "vkCmdSetFragmentShadingRateKHR");
            if (CmdSetFragmentShadingRateKHR == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetFragmentShadingRateKHR function");
                return false;
            }

            CmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorWriteEnableEXT");
            if (CmdSetColorWriteEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetColorWriteEnableEXT function");
                return false;
            }

            CmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorBlendEnableEXT");
            if (CmdSetColorBlendEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetColorBlendEnableEXT function");
                return false;
            }

            CmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorBlendEquationEXT");
            if (CmdSetColorBlendEquationEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetColorBlendEquationEXT function");
                return false;
            }

            CmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetPolygonModeEXT");
            if (CmdSetPolygonModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VulkanCommands: failed to load vkCmdSetPolygonModeEXT function");
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------
    }
}