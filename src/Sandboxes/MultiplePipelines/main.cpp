#include "multiple_pipelines_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_parameters.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "MultiplePipelines";
    }
    //--------------------------------------------------------------------------


    namespace Graphics
    {
        void InitializeMultiplePipelinesGraphicsParameters(GraphicsParameters& parameters)
        {
            if (parameters.type == GraphicsBackendType::Vulkan)
            {
                auto& vulkanParameters = dynamic_cast<VulkanGraphicsParameters&>(parameters);

                vulkanParameters.lineRasterizationFeatures.bresenhamLines = VK_TRUE;
                vulkanParameters.lineRasterizationFeatures.rectangularLines = VK_TRUE;
                vulkanParameters.lineRasterizationFeatures.smoothLines = VK_TRUE;
                vulkanParameters.lineRasterizationFeatures.stippledBresenhamLines = VK_TRUE;
                vulkanParameters.lineRasterizationFeatures.stippledRectangularLines = VK_TRUE;
                vulkanParameters.lineRasterizationFeatures.stippledSmoothLines = VK_TRUE;
                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3RasterizationSamples = VK_TRUE;
                vulkanParameters.features.fillModeNonSolid = VK_TRUE;
                vulkanParameters.features.wideLines = VK_TRUE;
                vulkanParameters.features13.dynamicRendering = VK_TRUE;
                vulkanParameters.features13.synchronization2 = VK_TRUE;

                vulkanParameters.features2.features = vulkanParameters.features;

                vulkanParameters.maxDescriptorSets = 1;
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
                    .applicationName = "Multiple Pipelines sandbox",
                    .settingsFilepath = programOptions.GetSettingsFilepath(),
                    .defaultSettingsFileName = "MultiplePipelinesSandbox_settings.json"
                },
                .resizable = true
            };

            Graphics::ClientInitializeGraphicsParametersFn = Graphics::InitializeMultiplePipelinesGraphicsParameters;

            return CreateUPtr<MultiplePipelinesApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}