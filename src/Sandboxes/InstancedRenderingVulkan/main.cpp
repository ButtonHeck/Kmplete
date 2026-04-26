#include "instanced_rendering_vulkan_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "InstancedRenderingVulkan";
    }
    //--------------------------------------------------------------------------


    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters =
                    {.applicationName = "Instanced Rendering Vulkan sandbox",
                     .settingsFilepath = programOptions.GetSettingsFilepath(),
                     .defaultSettingsFileName = "InstancedRenderingVulkan_settings.json"
                    },
                .resizable = true
            };

            return CreateUPtr<InstancedRenderingVulkanApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}