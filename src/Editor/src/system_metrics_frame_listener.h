#pragma once

#include "Kmplete/Application/application_frame_listener.h"
#include "Kmplete/Core/timer.h"

namespace Kmplete
{
    class SettingsDocument;
    class SystemMetricsManager;

    class SystemMetricsFrameListener : public ApplicationFrameListener
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