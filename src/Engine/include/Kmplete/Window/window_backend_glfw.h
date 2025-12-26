#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window_cursor.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    //! GLFW implementation of the window backend
    //! @see WindowBackend
    class WindowBackendGlfw : public WindowBackend
    {
        KMP_LOG_CLASSNAME(WindowBackendGlfw)

    public:
        KMP_DISABLE_COPY_MOVE(WindowBackendGlfw)

        KMP_API WindowBackendGlfw();
        KMP_API ~WindowBackendGlfw();

        KMP_NODISCARD KMP_API Window& CreateMainWindow() override;
        KMP_NODISCARD KMP_API Window& GetMainWindow() override;

        KMP_NODISCARD KMP_API Nullable<Window*> CreateAuxWindow(const String& windowName) override;
        KMP_NODISCARD KMP_API Nullable<Window*> CreateAuxWindow(const Window::WindowSettings& windowSettings) override;
        KMP_NODISCARD KMP_API Nullable<Window*> GetAuxWindow(const String& windowName) const override;
        KMP_API bool DestroyAuxWindow(const String& windowName) override;

        KMP_NODISCARD KMP_API int GetMonitorCount() const override;
        KMP_NODISCARD KMP_API StringVector GetMonitorNames() const override;
        KMP_NODISCARD KMP_API Vector<MonitorVideoMode> GetPrimaryMonitorVideoModes() const override;
        KMP_NODISCARD KMP_API Vector<MonitorVideoMode> GetMonitorVideoModes(int index) const override;
        KMP_NODISCARD KMP_API float GetPrimaryMonitorDPIScale() const override;

        KMP_NODISCARD KMP_API OptionalRef<const WindowCursor> AddCursor(const String& name, const Filepath& filepath, const Math::Point2I& hotspot = Math::Point2I()) override;
        KMP_NODISCARD KMP_API OptionalRef<const WindowCursor> GetCursor(const String& name) const override;

        KMP_API void SaveSettings(SettingsDocument& settings) const override;
        KMP_API void LoadSettings(SettingsDocument& settings) override;

    private:
        void _Initialize();
        void _InitializeCallbacks() const;
        void _InitializeErrorCallback() const;

        void _Finalize();

        void _SaveMainWindowSettings(SettingsDocument& settings) const;
        void _SaveAuxWindowsSettings(SettingsDocument& settings) const;

        void _LoadMainWindowSettings(SettingsDocument& settings);
        void _LoadAuxWindowsSettings(SettingsDocument& settings);

        KMP_NODISCARD bool _IsAuxWindowNameAcceptable(const String& name) const noexcept;

    private:
        UPtr<Window::WindowSettings> _mainWindowSettings;
        UPtr<Window> _mainWindow;

        HashMap<String, UPtr<Window::WindowSettings>> _auxWindowsSettings;
        HashMap<String, UPtr<Window>> _auxWindows;

        HashMap<String, UPtr<WindowCursor>> _cursors;
    };
    //--------------------------------------------------------------------------
}