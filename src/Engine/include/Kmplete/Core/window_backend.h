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
        constexpr static auto SettingsEntryName = "WindowBackend";
        constexpr static auto MainWindowStr = "MainWindow";
        constexpr static auto AuxWindowsStr = "AuxWindows";

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

        WindowBackend() = default;
        virtual ~WindowBackend() = default;

        KMP_NODISCARD KMP_API virtual Window& CreateMainWindow() = 0;
        KMP_NODISCARD KMP_API virtual Window& GetMainWindow() = 0;

        KMP_NODISCARD KMP_API virtual Nullable<Window*> CreateAuxWindow(const std::string& windowName) = 0;
        KMP_NODISCARD KMP_API virtual Nullable<Window*> CreateAuxWindow(Window::WindowSettings& windowSettings) = 0;
        KMP_NODISCARD KMP_API virtual Nullable<Window*> GetAuxWindow(const std::string& windowName) const = 0;
        KMP_API virtual bool DestroyAuxWindow(const std::string& windowName) = 0;

        KMP_NODISCARD KMP_API virtual int GetMonitorCount() const = 0;
        KMP_NODISCARD KMP_API virtual StringVector GetMonitorNames() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const = 0;
        KMP_NODISCARD KMP_API virtual std::vector<MonitorVideoMode> GetMonitorVideoModes(int index) const = 0;
        KMP_NODISCARD KMP_API virtual float GetDPIScale() const = 0;

        KMP_API virtual void SaveSettings(Settings& settings) const = 0;
        KMP_API virtual void LoadSettings(Settings& settings) = 0;
    };
    //--------------------------------------------------------------------------
}