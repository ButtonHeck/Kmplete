#include "uniform_buffers_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_parameters.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "UniformBuffers";
    }
    //--------------------------------------------------------------------------


    namespace Graphics
    {
        void InitializeUniformBuffersGraphicsParameters(GraphicsParameters& parameters)
        {
            if (parameters.type == GraphicsBackendType::Vulkan)
            {
                auto& vulkanParameters = dynamic_cast<VulkanGraphicsParameters&>(parameters);

                vulkanParameters.dynamicStateFeatures3.extendedDynamicState3RasterizationSamples = VK_TRUE;
                vulkanParameters.features13.dynamicRendering = VK_TRUE;
                vulkanParameters.features13.synchronization2 = VK_TRUE;

                vulkanParameters.descriptorPoolSizes = {
                    { VKBits::VK_DescriptorType_UniformBuffer, 1 },
                    { VKBits::VK_DescriptorType_UniformBufferDynamic, 1 }
                };
                vulkanParameters.maxDescriptorSets = 1 * NumConcurrentFrames;
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
                    .applicationName = "Uniform Buffers sandbox",
                    .settingsFilepath = programOptions.GetSettingsFilepath(),
                    .defaultSettingsFileName = "UniformBuffersSandbox_settings.json"
                },
                .resizable = true
            };

            Graphics::ClientInitializeGraphicsParametersFn = Graphics::InitializeUniformBuffersGraphicsParameters;

            return CreateUPtr<UniformBuffersApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}
