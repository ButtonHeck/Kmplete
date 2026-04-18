#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
        PFN_vkCreateDebugUtilsMessengerEXT VKCommands::CreateDebugUtilsMessengerEXT = nullptr;
        PFN_vkDestroyDebugUtilsMessengerEXT VKCommands::DestroyDebugUtilsMessengerEXT = nullptr;
#endif
        PFN_vkCmdSetRasterizationSamplesEXT VKCommands::CmdSetRasterizationSamplesEXT = nullptr;
        PFN_vkCmdSetDepthClampEnableEXT VKCommands::CmdSetDepthClampEnableEXT = nullptr;
        PFN_vkCmdSetDepthClipEnableEXT VKCommands::CmdSetDepthClipEnableEXT = nullptr;
        PFN_vkCmdSetDepthClampRangeEXT VKCommands::CmdSetDepthClampRangeEXT = nullptr;
        PFN_vkCmdSetLineStippleEnableEXT VKCommands::CmdSetLineStippleEnableEXT = nullptr;
        PFN_vkCmdSetLineRasterizationModeEXT VKCommands::CmdSetLineRasterizationModeEXT = nullptr;
        PFN_vkCmdSetDiscardRectangleEnableEXT VKCommands::CmdSetDiscardRectangleEnableEXT = nullptr;
        PFN_vkCmdSetDiscardRectangleEXT VKCommands::CmdSetDiscardRectangleEXT = nullptr;
        PFN_vkCmdSetDiscardRectangleModeEXT VKCommands::CmdSetDiscardRectangleModeEXT = nullptr;
        PFN_vkCmdSetSampleLocationsEnableEXT VKCommands::CmdSetSampleLocationsEnableEXT = nullptr;
        PFN_vkCmdSetSampleLocationsEXT VKCommands::CmdSetSampleLocationsEXT = nullptr;
        PFN_vkCmdSetSampleMaskEXT VKCommands::CmdSetSampleMaskEXT = nullptr;
        PFN_vkCmdSetFragmentShadingRateKHR VKCommands::CmdSetFragmentShadingRateKHR = nullptr;
        PFN_vkCmdSetColorWriteEnableEXT VKCommands::CmdSetColorWriteEnableEXT = nullptr;
        PFN_vkCmdSetColorWriteMaskEXT VKCommands::CmdSetColorWriteMaskEXT = nullptr;
        PFN_vkCmdSetColorBlendEnableEXT VKCommands::CmdSetColorBlendEnableEXT = nullptr;
        PFN_vkCmdSetColorBlendEquationEXT VKCommands::CmdSetColorBlendEquationEXT = nullptr;
        PFN_vkCmdSetPolygonModeEXT VKCommands::CmdSetPolygonModeEXT = nullptr;
        PFN_vkCmdSetAlphaToCoverageEnableEXT VKCommands::CmdSetAlphaToCoverageEnableEXT = nullptr;
        PFN_vkCmdSetAlphaToOneEnableEXT VKCommands::CmdSetAlphaToOneEnableEXT = nullptr;
        PFN_vkCmdSetLogicOpEnableEXT VKCommands::CmdSetLogicOpEnableEXT = nullptr;
        PFN_vkCmdSetLogicOpEXT VKCommands::CmdSetLogicOpEXT = nullptr;
        PFN_vkCmdSetProvokingVertexModeEXT VKCommands::CmdSetProvokingVertexModeEXT = nullptr;
        PFN_vkCmdSetVertexInputEXT VKCommands::CmdSetVertexInputEXT = nullptr;
        PFN_vkCreateShadersEXT VKCommands::CreateShadersEXT = nullptr;
        PFN_vkDestroyShaderEXT VKCommands::DestroyShaderEXT = nullptr;
        PFN_vkCmdBindShadersEXT VKCommands::CmdBindShadersEXT = nullptr;


        bool VKCommands::LoadExtensionFunctions(VkInstance instance) KMP_PROFILING(ProfileLevelAlways)
        {
#if not defined (KMP_CONFIG_TYPE_PRODUCTION)
            CreateDebugUtilsMessengerEXT = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
            if (CreateDebugUtilsMessengerEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCreateDebugUtilsMessengerEXT function");
                return false;
            }

            DestroyDebugUtilsMessengerEXT = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
            if (DestroyDebugUtilsMessengerEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkDestroyDebugUtilsMessengerEXT function");
                return false;
            }
#endif

            CmdSetRasterizationSamplesEXT = (PFN_vkCmdSetRasterizationSamplesEXT)vkGetInstanceProcAddr(instance, "vkCmdSetRasterizationSamplesEXT");
            if (CmdSetRasterizationSamplesEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetRasterizationSamplesEXT function");
                return false;
            }

            CmdSetDepthClampEnableEXT = (PFN_vkCmdSetDepthClampEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDepthClampEnableEXT");
            if (CmdSetDepthClampEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetDepthClampEnableEXT function");
                return false;
            }

            CmdSetDepthClipEnableEXT = (PFN_vkCmdSetDepthClipEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDepthClipEnableEXT");
            if (CmdSetDepthClipEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetDepthClipEnableEXT function");
                return false;
            }

            CmdSetDepthClampRangeEXT = (PFN_vkCmdSetDepthClampRangeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDepthClampRangeEXT");
            if (CmdSetDepthClampRangeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetDepthClampRangeEXT function");
                return false;
            }

            CmdSetLineStippleEnableEXT = (PFN_vkCmdSetLineStippleEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetLineStippleEnableEXT");
            if (CmdSetLineStippleEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetLineStippleEnableEXT function");
                return false;
            }

            CmdSetLineRasterizationModeEXT = (PFN_vkCmdSetLineRasterizationModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetLineRasterizationModeEXT");
            if (CmdSetLineRasterizationModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetLineRasterizationModeEXT function");
                return false;
            }

            CmdSetDiscardRectangleEnableEXT = (PFN_vkCmdSetDiscardRectangleEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDiscardRectangleEnableEXT");
            if (CmdSetDiscardRectangleEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetDiscardRectangleEnableEXT function");
                return false;
            }

            CmdSetDiscardRectangleEXT = (PFN_vkCmdSetDiscardRectangleEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDiscardRectangleEXT");
            if (CmdSetDiscardRectangleEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetDiscardRectangleEXT function");
                return false;
            }

            CmdSetDiscardRectangleModeEXT = (PFN_vkCmdSetDiscardRectangleModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetDiscardRectangleModeEXT");
            if (CmdSetDiscardRectangleModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetDiscardRectangleModeEXT function");
                return false;
            }

            CmdSetSampleLocationsEnableEXT = (PFN_vkCmdSetSampleLocationsEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetSampleLocationsEnableEXT");
            if (CmdSetSampleLocationsEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetSampleLocationsEnableEXT function");
                return false;
            }

            CmdSetSampleLocationsEXT = (PFN_vkCmdSetSampleLocationsEXT)vkGetInstanceProcAddr(instance, "vkCmdSetSampleLocationsEXT");
            if (CmdSetSampleLocationsEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetSampleLocationsEXT function");
                return false;
            }

            CmdSetSampleMaskEXT = (PFN_vkCmdSetSampleMaskEXT)vkGetInstanceProcAddr(instance, "vkCmdSetSampleMaskEXT");
            if (CmdSetSampleMaskEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetSampleMaskEXT function");
                return false;
            }

            CmdSetFragmentShadingRateKHR = (PFN_vkCmdSetFragmentShadingRateKHR)vkGetInstanceProcAddr(instance, "vkCmdSetFragmentShadingRateKHR");
            if (CmdSetFragmentShadingRateKHR == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetFragmentShadingRateKHR function");
                return false;
            }

            CmdSetColorWriteEnableEXT = (PFN_vkCmdSetColorWriteEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorWriteEnableEXT");
            if (CmdSetColorWriteEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetColorWriteEnableEXT function");
                return false;
            }

            CmdSetColorWriteMaskEXT = (PFN_vkCmdSetColorWriteMaskEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorWriteMaskEXT");
            if (CmdSetColorWriteMaskEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetColorWriteMaskEXT function");
                return false;
            }

            CmdSetColorBlendEnableEXT = (PFN_vkCmdSetColorBlendEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorBlendEnableEXT");
            if (CmdSetColorBlendEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetColorBlendEnableEXT function");
                return false;
            }

            CmdSetColorBlendEquationEXT = (PFN_vkCmdSetColorBlendEquationEXT)vkGetInstanceProcAddr(instance, "vkCmdSetColorBlendEquationEXT");
            if (CmdSetColorBlendEquationEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetColorBlendEquationEXT function");
                return false;
            }

            CmdSetPolygonModeEXT = (PFN_vkCmdSetPolygonModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetPolygonModeEXT");
            if (CmdSetPolygonModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetPolygonModeEXT function");
                return false;
            }

            CmdSetAlphaToCoverageEnableEXT = (PFN_vkCmdSetAlphaToCoverageEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetAlphaToCoverageEnableEXT");
            if (CmdSetAlphaToCoverageEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetAlphaToCoverageEnableEXT function");
                return false;
            }

            CmdSetAlphaToOneEnableEXT = (PFN_vkCmdSetAlphaToOneEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetAlphaToOneEnableEXT");
            if (CmdSetAlphaToOneEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetAlphaToOneEnableEXT function");
                return false;
            }

            CmdSetLogicOpEnableEXT = (PFN_vkCmdSetLogicOpEnableEXT)vkGetInstanceProcAddr(instance, "vkCmdSetLogicOpEnableEXT");
            if (CmdSetLogicOpEnableEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetLogicOpEnableEXT function");
                return false;
            }

            CmdSetLogicOpEXT = (PFN_vkCmdSetLogicOpEXT)vkGetInstanceProcAddr(instance, "vkCmdSetLogicOpEXT");
            if (CmdSetLogicOpEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetLogicOpEXT function");
                return false;
            }

            CmdSetProvokingVertexModeEXT = (PFN_vkCmdSetProvokingVertexModeEXT)vkGetInstanceProcAddr(instance, "vkCmdSetProvokingVertexModeEXT");
            if (CmdSetProvokingVertexModeEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetProvokingVertexModeEXT function");
                return false;
            }

            CmdSetVertexInputEXT = (PFN_vkCmdSetVertexInputEXT)vkGetInstanceProcAddr(instance, "vkCmdSetVertexInputEXT");
            if (CmdSetVertexInputEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdSetVertexInputEXT function");
                return false;
            }

            CreateShadersEXT = (PFN_vkCreateShadersEXT)vkGetInstanceProcAddr(instance, "vkCreateShadersEXT");
            if (CreateShadersEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCreateShadersEXT function");
                return false;
            }

            DestroyShaderEXT = (PFN_vkDestroyShaderEXT)vkGetInstanceProcAddr(instance, "vkDestroyShaderEXT");
            if (DestroyShaderEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCreateShadersEXT function");
                return false;
            }

            CmdBindShadersEXT = (PFN_vkCmdBindShadersEXT)vkGetInstanceProcAddr(instance, "vkCmdBindShadersEXT");
            if (CmdBindShadersEXT == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load vkCmdBindShadersEXT function");
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------
    }
}