#include "storage_buffers_application.h"

#include "Kmplete/Core/main.h"
#include "Kmplete/Base/exception.h"


namespace Kmplete
{
    const char* ApplicationProfileSessionPrefix()
    {
        return "StorageBuffers";
    }
    //--------------------------------------------------------------------------


    UPtr<Application> CreateApplication(const ProgramOptions& programOptions) KMP_PROFILING(ProfileLevelAlways)
    {
        try
        {
            WindowApplicationParameters parameters = WindowApplicationParameters{
                .applicationParameters = {
                    .applicationName = "Storage Buffers sandbox",
                    .settingsFilepath = programOptions.GetSettingsFilepath(),
                    .defaultSettingsFileName = "StorageBuffersSandbox_settings.json"
                },
                .resizable = true
            };

            return CreateUPtr<StorageBuffersApplication>(parameters);
        }
        catch (const Exception&)
        {
            KMP_LOG_CRITICAL_FN("CreateApplication: failed to create sandbox instance");
            return nullptr;
        }
    }}
    //--------------------------------------------------------------------------
}
