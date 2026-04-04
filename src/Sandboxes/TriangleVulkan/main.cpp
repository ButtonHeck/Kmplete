#include "triangle_vulkan_application.h"

#include "Kmplete/Core/main.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "TriangleVulkan";
    }
    //--------------------------------------------------------------------------


    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters =
                    {.applicationName = "Triangle Vulkan sandbox",
                     .settingsFilepath = programOptions.GetSettingsFilepath(),
                     .defaultSettingsFileName = "TriangleVulkan_settings.json"
                    },
                .resizable = true
            };

            return CreateUPtr<TriangleVulkanApplication>(parameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}