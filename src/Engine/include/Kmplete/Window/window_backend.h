#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/geometry.h"


namespace Kmplete
{
    class SettingsDocument;
    class WindowCursor;


    class WindowBackend
    {
    protected:
        static constexpr auto SettingsEntryName = "WindowBackend";
        static constexpr auto MainWindowStr = "MainWindow";
        static constexpr auto AuxWindowsStr = "AuxWindows";

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
        KMP_NODISCARD KMP_API virtual Nullable<Window*> CreateAuxWindow(const Window::WindowSettings& windowSettings) = 0;
        KMP_NODISCARD KMP_API virtual Nullable<Window*> GetAuxWindow(const String& windowName) const = 0;
        KMP_API virtual bool DestroyAuxWindow(const String& windowName) = 0;

        KMP_NODISCARD KMP_API virtual int GetMonitorCount() const = 0;
        KMP_NODISCARD KMP_API virtual StringVector GetMonitorNames() const = 0;
        KMP_NODISCARD KMP_API virtual Vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const = 0;
        KMP_NODISCARD KMP_API virtual Vector<MonitorVideoMode> GetMonitorVideoModes(int index) const = 0;
        KMP_NODISCARD KMP_API virtual float GetPrimaryMonitorDPIScale() const = 0;

        KMP_NODISCARD KMP_API virtual OptionalRef<const WindowCursor> AddCursor(const String& name, const Filepath& filepath, const Math::Point2I& hotspot = Math::Point2I()) = 0;
        KMP_NODISCARD KMP_API virtual OptionalRef<const WindowCursor> GetCursor(const String& name) const = 0;

        KMP_API virtual void SaveSettings(SettingsDocument& settings) const = 0;
        KMP_API virtual void LoadSettings(SettingsDocument& settings) = 0;
    };
    //--------------------------------------------------------------------------
}