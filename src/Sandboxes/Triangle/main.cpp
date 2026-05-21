#include "triangle_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_parameters.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "Triangle";
    }
    //--------------------------------------------------------------------------


    namespace Graphics
    {
        void InitializeTriangleGraphicsParameters(GraphicsParameters& parameters)
        {
            if (parameters.type == GraphicsBackendType::Vulkan)
            {
                auto& vulkanParameters = dynamic_cast<VulkanGraphicsParameters&>(parameters);

                vulkanParameters.vertexAttributeDivisorFeatures.vertexAttributeInstanceRateDivisor = VK_FALSE;
                vulkanParameters.vertexAttributeDivisorFeatures.vertexAttributeInstanceRateZeroDivisor = VK_FALSE;

                vulkanParameters.lineRasterizationFeatures.bresenhamLines = VK_FALSE;
                vulkanParameters.lineRasterizationFeatures.rectangularLines = VK_FALSE;
                vulkanParameters.lineRasterizationFeatures.smoothLines = VK_FALSE;
                vulkanParameters.lineRasterizationFeatures.stippledBresenhamLines = VK_FALSE;
                vulkanParameters.lineRasterizationFeatures.stippledRectangularLines = VK_FALSE;
                vulkanParameters.lineRasterizationFeatures.stippledSmoothLines = VK_FALSE;

                vulkanParameters.shaderObjectFeatures.shaderObject = VK_TRUE;

                vulkanParameters.vertexInputDynamicStateFeatures.vertexInputDynamicState = VK_TRUE;

                vulkanParameters.dynamicStateFeatures2.extendedDynamicState2LogicOp = VK_TRUE;

                vulkanParameters.colorWriteEnableFeatures.colorWriteEnable = VK_TRUE;

                vulkanParameters.depthClipEnableFeatures.depthClipEnable = VK_TRUE;

                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3ColorBlendEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3RasterizationSamples = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3DepthClampEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3DepthClipEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3LineStippleEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3LineRasterizationMode = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3SampleLocationsEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3SampleMask = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3PolygonMode = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3ColorBlendEquation = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3ColorWriteMask = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3AlphaToCoverageEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3AlphaToOneEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3LogicOpEnable = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3ProvokingVertexMode = VK_TRUE;

                vulkanParameters.features.samplerAnisotropy = VK_FALSE;
                vulkanParameters.features.independentBlend = VK_FALSE;
                vulkanParameters.features.depthBounds = VK_FALSE;
                vulkanParameters.features.fillModeNonSolid = VK_FALSE;
                vulkanParameters.features.alphaToOne = VK_FALSE;
                vulkanParameters.features.logicOp = VK_FALSE;
                vulkanParameters.features.wideLines = VK_FALSE;

                vulkanParameters.features13.dynamicRendering = VK_TRUE;
                vulkanParameters.features13.synchronization2 = VK_TRUE;

                vulkanParameters.features2.features = vulkanParameters.features;
            }
        }
        //--------------------------------------------------------------------------
    }


    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters = {
                    .applicationName = "Triangle sandbox",
                    .settingsFilepath = programOptions.GetSettingsFilepath(),
                    .defaultSettingsFileName = "TriangleSandbox_settings.json"
                },
                .resizable = true
            };

            Graphics::ClientInitializeGraphicsParametersFn = Graphics::InitializeTriangleGraphicsParameters;

            return CreateUPtr<TriangleApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}