#include "uniform_buffers_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "UniformBuffers";
    }
    //--------------------------------------------------------------------------


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
