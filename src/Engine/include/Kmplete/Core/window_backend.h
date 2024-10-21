#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/types.h"

namespace Kmplete
{
    class Settings;

    class WindowBackend
    {
    protected:
        constexpr static auto WindowBackendSettingsEntryName = "WindowBackend";
        constexpr static auto WindowsStr = "Windows";

    public:
        struct MonitorVideoMode
        {
            int width;
            int height;
            int refreshRate;
        };

    public:
        KMP_NODISCARD KMP_API static UPtr<WindowBackend> Create();

    public:
        KMP_DISABLE_COPY_MOVE(WindowBackend)

        KMP_API WindowBackend() = default;
        KMP_API virtual ~WindowBackend() = default;

        KMP_NODISCARD KMP_API virtual Ptr<Window> CreateWindow(const std::string& windowName) = 0;
        KMP_NODISCARD KMP_API virtual Ptr<Window> CreateWindow(const Ptr<Window::WindowSettings> windowSettings) = 0;

        KMP_NODISCARD KMP_API virtual int GetMonitorCount() const = 0;
        KMP_NODISCARD KMP_API virtual StringVector GetMonitorNames() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<MonitorVideoMode> GetMonitorVideoModes(int index) const = 0;

        KMP_API virtual void SaveSettings(const Ptr<Settings> settings) const = 0;
        KMP_API virtual void LoadSettings(const Ptr<Settings> settings) = 0;
    };
    //--------------------------------------------------------------------------
}