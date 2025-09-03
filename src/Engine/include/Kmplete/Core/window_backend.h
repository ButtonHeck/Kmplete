#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Core/window.h"

namespace Kmplete
{
    class Settings;
    class WindowCursor;

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

        KMP_NODISCARD KMP_API virtual Nullable<Window*> CreateAuxWindow(const String& windowName) = 0;
        KMP_NODISCARD KMP_API virtual Nullable<Window*> CreateAuxWindow(Window::WindowSettings& windowSettings) = 0;
        KMP_NODISCARD KMP_API virtual Nullable<Window*> GetAuxWindow(const String& windowName) const = 0;
        KMP_API virtual bool DestroyAuxWindow(const String& windowName) = 0;

        KMP_NODISCARD KMP_API virtual int GetMonitorCount() const = 0;
        KMP_NODISCARD KMP_API virtual StringVector GetMonitorNames() const = 0;
        KMP_NODISCARD KMP_API virtual Vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const = 0;
        KMP_NODISCARD KMP_API virtual Vector<MonitorVideoMode> GetMonitorVideoModes(int index) const = 0;
        KMP_NODISCARD KMP_API virtual float GetPrimaryMonitorDPIScale() const = 0;

        // TODO: try replace return type with optional<cursor&>
        KMP_NODISCARD KMP_API virtual Nullable<WindowCursor*> AddCursor(const String& name, const Filepath& filepath, int hotspotX = 0, int hotspotY = 0) = 0;
        KMP_NODISCARD KMP_API virtual Nullable<WindowCursor*> GetCursor(const String& name) const = 0;

        KMP_API virtual void SaveSettings(Settings& settings) const = 0;
        KMP_API virtual void LoadSettings(Settings& settings) = 0;
    };
    //--------------------------------------------------------------------------
}