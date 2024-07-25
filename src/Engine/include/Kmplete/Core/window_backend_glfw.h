#pragma once

#include "Kmplete/Core/window_backend.h"

namespace Kmplete
{
    class WindowBackendGlfw : public WindowBackend
    {
    public:
        KMP_API ~WindowBackendGlfw();

        KMP_API KMP_NODISCARD bool Initialize() const override;
        KMP_API void Finalize() const override;

        KMP_API KMP_NODISCARD Ptr<Window> CreateWindow() const override;

        KMP_API KMP_NODISCARD int GetMonitorCount() const override;
        KMP_API KMP_NODISCARD std::vector<std::string> GetMonitorNames() const override;
        KMP_API KMP_NODISCARD std::vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const override;
        KMP_API KMP_NODISCARD std::vector<MonitorVideoMode> GetMonitorVideoModes(int index) const override;
    };
    //--------------------------------------------------------------------------
}