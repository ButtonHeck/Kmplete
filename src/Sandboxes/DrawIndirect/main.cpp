#include "draw_indirect_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Graphics/Vulkan/Core/vulkan_graphics_parameters.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "DrawIndirect";
    }
    //--------------------------------------------------------------------------


    namespace Graphics
    {
        void InitializeDrawIndirectGraphicsParameters(GraphicsParameters& parameters)
        {
            if (parameters.type == GraphicsBackendType::Vulkan)
            {
                auto& vulkanParameters = dynamic_cast<VulkanGraphicsParameters&>(parameters);

                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3RasterizationSamples = VK_TRUE;
                vulkanParameters.features13.dynamicRendering = VK_TRUE;
                vulkanParameters.features13.synchronization2 = VK_TRUE;
                vulkanParameters.features2.features.multiDrawIndirect = VK_TRUE;

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
                    .applicationName = "Draw indirect sandbox",
                    .settingsFilepath = programOptions.GetSettingsFilepath(),
                    .defaultSettingsFileName = "DrawIndirectSandbox_settings.json"
                },
                .resizable = true
            };

            Graphics::ClientInitializeGraphicsParametersFn = Graphics::InitializeDrawIndirectGraphicsParameters;

            return CreateUPtr<DrawIndirectApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}
