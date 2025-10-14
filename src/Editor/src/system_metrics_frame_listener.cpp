#include "system_metrics_frame_listener.h"

#include "Kmplete/Core/system_metrics_manager.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Profile/profiler.h"


namespace Kmplete
{
    constexpr static auto SettingsEntryName = "SystemMetricsFrameListener";
    constexpr static auto MetricsTimeoutStr = "MetricsTimeout";

    SystemMetricsFrameListener::SystemMetricsFrameListener(SystemMetricsManager& metricsManager)
        : FrameListener("SystemMetrics")
        , _metricsManager(metricsManager)
        , _timer(1000)
    {}
    //--------------------------------------------------------------------------

    void SystemMetricsFrameListener::Update(KMP_MB_UNUSED float frameTimestep, KMP_MB_UNUSED bool applicationIsIconified)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        if (_timer.ReachedTimeout())
        {
            _timer.Mark();
            _metricsManager.Update(SystemMetricsManager::SystemMetricsUpdateMode::MemoryAndCPU);
        }
    }
    //--------------------------------------------------------------------------

    Timer& SystemMetricsFrameListener::GetTimer()
    {
        return _timer;
    }
    //--------------------------------------------------------------------------

    void SystemMetricsFrameListener::SaveSettings(SettingsDocument& settings) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartSaveObject(SettingsEntryName);
        settings.SaveUInt(MetricsTimeoutStr, _timer.GetTimeout());
        settings.EndSaveObject();
    }
    //--------------------------------------------------------------------------

    void SystemMetricsFrameListener::LoadSettings(SettingsDocument& settings)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        settings.StartLoadObject(SettingsEntryName);
        _timer.SetTimeout(settings.GetUInt(MetricsTimeoutStr, 1000));
        settings.EndLoadObject();
    }
    //--------------------------------------------------------------------------
}