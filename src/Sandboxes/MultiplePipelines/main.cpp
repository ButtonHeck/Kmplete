#include "multiple_pipelines_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "MultiplePipelines";
    }
    //--------------------------------------------------------------------------


    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters =
                    {.applicationName = "Multiple Pipelines sandbox",
                     .settingsFilepath = programOptions.GetSettingsFilepath(),
                     .defaultSettingsFileName = "MultiplePipelinesSandbox_settings.json"
                    },
                .resizable = true
            };

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