#include "texture_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"
#include "Kmplete/Graphics/Vulkan/vulkan_graphics_parameters.h"
#include "Kmplete/Graphics/Vulkan/Utils/bits_aliases.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "Texture";
    }
    //--------------------------------------------------------------------------


    namespace Graphics
    {
        void InitializeTextureGraphicsParameters(GraphicsParameters& parameters)
        {
            if (parameters.type == GraphicsBackendType::Vulkan)
            {
                auto& vulkanParameters = dynamic_cast<VulkanGraphicsParameters&>(parameters);

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
                vulkanParameters.features13.dynamicRendering = VK_TRUE;
                vulkanParameters.features13.synchronization2 = VK_TRUE;

                vulkanParameters.descriptorPoolSizes = {
                    { VKBits::VK_DescriptorType_UniformBuffer, 1 },
                    { VKBits::VK_DescriptorType_SampledImage, 1 },
                    { VKBits::VK_DescriptorType_Sampler, 1 }
                };
                vulkanParameters.maxDescriptorSets = 2 * NumConcurrentFrames;
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
                    .applicationName = "Texture sandbox",
                    .settingsFilepath = programOptions.GetSettingsFilepath(),
                    .defaultSettingsFileName = "TextureSandbox_settings.json"
                },
                .resizable = true
            };

            Graphics::ClientInitializeGraphicsParametersFn = Graphics::InitializeTextureGraphicsParameters;

            return CreateUPtr<TextureApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}
