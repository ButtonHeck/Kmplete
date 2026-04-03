#include "Application/editor_application.h"

#include "Kmplete/Core/main.h"


namespace Kmplete
{
    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters = 
                    {.applicationName = "Kmplete Editor",
                     .settingsFilepath = programOptions.GetSettingsFilepath()
                    },
                .resizable = true
            };

            return CreateUPtr<EditorApplication>(parameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create Editor instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}