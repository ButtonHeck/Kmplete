#include "uniform_dynamic_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "UniformDynamic";
    }
    //--------------------------------------------------------------------------


    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters =
                    {.applicationName = "Uniform Dynamic sandbox",
                     .settingsFilepath = programOptions.GetSettingsFilepath(),
                     .defaultSettingsFileName = "UniformDynamicSandbox_settings.json"
                    },
                .resizable = true
            };

            return CreateUPtr<UniformDynamicApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }
}
//--------------------------------------------------------------------------
}
