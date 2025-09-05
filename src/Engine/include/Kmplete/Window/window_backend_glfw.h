#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Window/window_cursor.h"
#include "Kmplete/Log/log_class_macro.h"

#include <unordered_map>

namespace Kmplete
{
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

        KMP_NODISCARD KMP_API OptionalRef<const WindowCursor> AddCursor(const String& name, const Filepath& filepath, int hotspotX = 0, int hotspotY = 0) override;
        KMP_NODISCARD KMP_API OptionalRef<const WindowCursor> GetCursor(const String& name) const override;

        KMP_API void SaveSettings(Settings& settings) const override;
        KMP_API void LoadSettings(Settings& settings) override;

    private:
        void Initialize();
        void InitializeCallbacks() const;
        void InitializeErrorCallback() const;

        void Finalize();

        void SaveMainWindowSettings(Settings& settings) const;
        void SaveAuxWindowsSettings(Settings& settings) const;

        void LoadMainWindowSettings(Settings& settings);
        void LoadAuxWindowsSettings(Settings& settings);

    private:
        UPtr<Window::WindowSettings> _mainWindowSettings;
        UPtr<Window> _mainWindow;

        std::unordered_map<String, UPtr<Window::WindowSettings>> _auxWindowsSettings;
        std::unordered_map<String, UPtr<Window>> _auxWindows;

        std::unordered_map<String, UPtr<WindowCursor>> _cursors;
    };
    //--------------------------------------------------------------------------
}