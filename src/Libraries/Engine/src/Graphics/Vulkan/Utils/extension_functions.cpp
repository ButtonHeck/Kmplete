#include "Kmplete/Graphics/Vulkan/Utils/extension_functions.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    namespace Graphics
    {
        template<typename FunctionType>
        KMP_NODISCARD static bool LoadExtensionFunction(FunctionType& functionPointer, VkInstance instance, const char* functionName) KMP_PROFILING(ProfileLevelImportant)
        {
            functionPointer = (FunctionType)vkGetInstanceProcAddr(instance, functionName);
            if (functionPointer == nullptr)
            {
                KMP_LOG_CRITICAL_FN("VKCommands: failed to load {} function", functionName);
                return false;
            }
            return true;
        }}
        //--------------------------------------------------------------------------


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
            if (not LoadExtensionFunction<PFN_vkCreateDebugUtilsMessengerEXT>(CreateDebugUtilsMessengerEXT, instance, "vkCreateDebugUtilsMessengerEXT") ||
                not LoadExtensionFunction<PFN_vkDestroyDebugUtilsMessengerEXT>(DestroyDebugUtilsMessengerEXT, instance, "vkDestroyDebugUtilsMessengerEXT"))
            {
                return false;
            }
#endif

            if (not LoadExtensionFunction<PFN_vkCmdSetRasterizationSamplesEXT>(CmdSetRasterizationSamplesEXT, instance, "vkCmdSetRasterizationSamplesEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetDepthClampEnableEXT>(CmdSetDepthClampEnableEXT, instance, "vkCmdSetDepthClampEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetDepthClipEnableEXT>(CmdSetDepthClipEnableEXT, instance, "vkCmdSetDepthClipEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetDepthClampRangeEXT>(CmdSetDepthClampRangeEXT, instance, "vkCmdSetDepthClampRangeEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetLineStippleEnableEXT>(CmdSetLineStippleEnableEXT, instance, "vkCmdSetLineStippleEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetLineRasterizationModeEXT>(CmdSetLineRasterizationModeEXT, instance, "vkCmdSetLineRasterizationModeEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetDiscardRectangleEnableEXT>(CmdSetDiscardRectangleEnableEXT, instance, "vkCmdSetDiscardRectangleEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetDiscardRectangleEXT>(CmdSetDiscardRectangleEXT, instance, "vkCmdSetDiscardRectangleEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetDiscardRectangleModeEXT>(CmdSetDiscardRectangleModeEXT, instance, "vkCmdSetDiscardRectangleModeEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetSampleLocationsEnableEXT>(CmdSetSampleLocationsEnableEXT, instance, "vkCmdSetSampleLocationsEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetSampleLocationsEXT>(CmdSetSampleLocationsEXT, instance, "vkCmdSetSampleLocationsEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetSampleMaskEXT>(CmdSetSampleMaskEXT, instance, "vkCmdSetSampleMaskEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetFragmentShadingRateKHR>(CmdSetFragmentShadingRateKHR, instance, "vkCmdSetFragmentShadingRateKHR") ||
                not LoadExtensionFunction<PFN_vkCmdSetColorWriteEnableEXT>(CmdSetColorWriteEnableEXT, instance, "vkCmdSetColorWriteEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetColorWriteMaskEXT>(CmdSetColorWriteMaskEXT, instance, "vkCmdSetColorWriteMaskEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetColorBlendEnableEXT>(CmdSetColorBlendEnableEXT, instance, "vkCmdSetColorBlendEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetColorBlendEquationEXT>(CmdSetColorBlendEquationEXT, instance, "vkCmdSetColorBlendEquationEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetPolygonModeEXT>(CmdSetPolygonModeEXT, instance, "vkCmdSetPolygonModeEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetAlphaToCoverageEnableEXT>(CmdSetAlphaToCoverageEnableEXT, instance, "vkCmdSetAlphaToCoverageEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetAlphaToOneEnableEXT>(CmdSetAlphaToOneEnableEXT, instance, "vkCmdSetAlphaToOneEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetLogicOpEnableEXT>(CmdSetLogicOpEnableEXT, instance, "vkCmdSetLogicOpEnableEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetLogicOpEXT>(CmdSetLogicOpEXT, instance, "vkCmdSetLogicOpEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetProvokingVertexModeEXT>(CmdSetProvokingVertexModeEXT, instance, "vkCmdSetProvokingVertexModeEXT") ||
                not LoadExtensionFunction<PFN_vkCmdSetVertexInputEXT>(CmdSetVertexInputEXT, instance, "vkCmdSetVertexInputEXT") ||
                not LoadExtensionFunction<PFN_vkCreateShadersEXT>(CreateShadersEXT, instance, "vkCreateShadersEXT") ||
                not LoadExtensionFunction<PFN_vkDestroyShaderEXT>(DestroyShaderEXT, instance, "vkDestroyShaderEXT") ||
                not LoadExtensionFunction<PFN_vkCmdBindShadersEXT>(CmdBindShadersEXT, instance, "vkCmdBindShadersEXT"))
            {
                return false;
            }

            return true;
        }}
        //--------------------------------------------------------------------------
    }
}