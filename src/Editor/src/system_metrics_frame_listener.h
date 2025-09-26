#pragma once

#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Core/timer.h"


namespace Kmplete
{
    class SettingsDocument;
    class SystemMetricsManager;

    class SystemMetricsFrameListener : public FrameListener
    {
    public:
        explicit SystemMetricsFrameListener(SystemMetricsManager& metricsManager);

        void Update(float frameTimestep, bool applicationIsIconified) override;

        KMP_NODISCARD Timer& GetTimer();

        void SaveSettings(SettingsDocument& settings) const;
        void LoadSettings(SettingsDocument& settings);

    private:
        SystemMetricsManager& _metricsManager;
        Timer _timer;
    };
    //--------------------------------------------------------------------------
}