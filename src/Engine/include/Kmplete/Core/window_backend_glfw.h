#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/window_backend.h"

#include <unordered_map>

namespace Kmplete
{
    class WindowBackendGlfw : public WindowBackend
    {
    public:
        KMP_DISABLE_COPY_MOVE(WindowBackendGlfw)

        KMP_API WindowBackendGlfw();
        KMP_API ~WindowBackendGlfw();

        KMP_NODISCARD KMP_API Ptr<Window> CreateWindow(const std::string& windowName) override;
        KMP_NODISCARD KMP_API Ptr<Window> CreateWindow(const Ptr<Window::WindowSettings> windowSettings) override;

        KMP_NODISCARD KMP_API int GetMonitorCount() const override;
        KMP_NODISCARD KMP_API StringVector GetMonitorNames() const override;
        KMP_NODISCARD KMP_API std::vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const override;
        KMP_NODISCARD KMP_API std::vector<MonitorVideoMode> GetMonitorVideoModes(int index) const override;

        KMP_API void SaveSettings(const Ptr<Settings> settings) const override;
        KMP_API void LoadSettings(const Ptr<Settings> settings) override;

    private:
        void Initialize();
        void Finalize();

    private:
        std::unordered_map<std::string, Ptr<Window::WindowSettings>> _windowsSettings;
    };
    //--------------------------------------------------------------------------
}