#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/optional.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Graphics/graphics_base.h"


namespace Kmplete
{
    class SettingsDocument;
    class WindowCursor;


    //! Base class for a window backend, responsible for initializing underlying implementation API, 
    //! creating/deleting window objects (by their names or settings object), cursors (by their names), interacting with monitors. 
    //! @see Window
    //! @see WindowCursor
    class KMP_API WindowBackend
    {
        KMP_DISABLE_COPY_MOVE(WindowBackend)

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

        enum NativePlatformType
        {
            Win32,
            X11,
            Wayland,
            Cocoa,
            Undefined
        };

    public:
        KMP_NODISCARD static UPtr<WindowBackend> Create(Graphics::GraphicsBackendType graphicsBackendType);

    public:
        explicit WindowBackend(Graphics::GraphicsBackendType graphicsBackendType) noexcept;
        virtual ~WindowBackend() = default;

        KMP_NODISCARD NativePlatformType GetNativePlatformType() const noexcept;

        KMP_NODISCARD virtual Window& CreateMainWindow() = 0;
        KMP_NODISCARD virtual Window& GetMainWindow() = 0;

        KMP_NODISCARD virtual Nullable<Window*> CreateAuxWindow(const String& windowName) = 0;
        KMP_NODISCARD virtual Nullable<Window*> CreateAuxWindow(const Window::WindowSettings& windowSettings) = 0;
        KMP_NODISCARD virtual Nullable<Window*> GetAuxWindow(const String& windowName) const = 0;
        virtual bool DestroyAuxWindow(const String& windowName) = 0;

        KMP_NODISCARD virtual int GetMonitorCount() const = 0;
        KMP_NODISCARD virtual StringVector GetMonitorNames() const = 0;
        KMP_NODISCARD virtual Vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const = 0;
        KMP_NODISCARD virtual Vector<MonitorVideoMode> GetMonitorVideoModes(unsigned int index) const = 0;
        KMP_NODISCARD virtual float GetPrimaryMonitorDPIScale() const = 0;

        KMP_NODISCARD virtual OptionalRef<const WindowCursor> AddCursor(const String& name, const Filepath& filepath, const Math::Point2I& hotspot = Math::Point2I()) = 0;
        KMP_NODISCARD virtual OptionalRef<const WindowCursor> GetCursor(const String& name) const = 0;

        virtual void SaveSettings(SettingsDocument& settings) const = 0;
        virtual void LoadSettings(SettingsDocument& settings) = 0;

    protected:
        Graphics::GraphicsBackendType _graphicsBackendType;
        NativePlatformType _nativePlatformType;
    };
    //--------------------------------------------------------------------------
}