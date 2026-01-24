#include "triangle_application.h"

#include "Kmplete/Core/main.h"


namespace Kmplete
{
    UPtr<Application> CreateApplication(const ProgramOptions& programOptions)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters =
                    {.applicationName = "Triangle OpenGL sandbox",
                     .settingsFilepath = programOptions.GetSettingsFilepath(),
                     .defaultSettingsFileName = "Triangle_app_settings.json"
                    },
                .resizable = true
            };

            return CreateUPtr<TriangleApplication>(parameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------
}