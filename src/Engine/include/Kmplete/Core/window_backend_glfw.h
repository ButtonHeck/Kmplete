#pragma once

#include "Kmplete/Core/window_backend.h"

namespace Kmplete
{
    class WindowBackendGlfw : public WindowBackend
    {
    public:
        KMP_API ~WindowBackendGlfw();

        KMP_NODISCARD KMP_API bool Initialize() const override;
        KMP_API void Finalize() const override;

        KMP_NODISCARD KMP_API Ptr<Window> CreateWindow() const override;

        KMP_NODISCARD KMP_API int GetMonitorCount() const override;
        KMP_NODISCARD KMP_API std::vector<std::string> GetMonitorNames() const override;
        KMP_NODISCARD KMP_API std::vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const override;
        KMP_NODISCARD KMP_API std::vector<MonitorVideoMode> GetMonitorVideoModes(int index) const override;
    };
    //--------------------------------------------------------------------------
}