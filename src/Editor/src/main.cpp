#include "editor_application.h"
#include "Kmplete/Core/main.h"

namespace Kmplete
{
    UPtr<Application> CreateApplication(const ProgramOptions& programOptions)
    {
        KMP_PROFILE_FUNCTION();

        try
        {
            ApplicationParameters applicationParameters{
                .applicationName = "Kmplete Editor",
                .settingsFilepath = programOptions.GetSettingsFilepath()
            };

            return CreateUPtr<EditorApplication>(applicationParameters);
        }
        catch (const std::exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create Editor instance");
            return nullptr;
        }
    }
    //--------------------------------------------------------------------------
}