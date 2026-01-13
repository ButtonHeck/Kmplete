#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class WindowCursor;
    class SettingsDocument;
    class Image;


    //! Base class for application window. Communication with the application is
    //! done via callback mechanism. Each WindowApplication-derived class may assume
    //! that it has an already created window named "Main".
    class Window
    {
        KMP_LOG_CLASSNAME(Window)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(Window)

    public:
        using EventCallbackFn = Function<void(Events::Event&)>;

    public:
        enum class ScreenMode
        {
            Fullscreen,
            WindowedFullscreen,
            Windowed
        };

        static constexpr auto FullscreenModeStr = "Fullscreen";
        static constexpr auto WindowedFullscreenModeStr = "WindowedFullscreen";
        static constexpr auto WindowedModeStr = "Windowed";

        KMP_NODISCARD KMP_API static String ScreenModeToString(ScreenMode screenMode) noexcept;
        KMP_NODISCARD KMP_API static ScreenMode StringToScreenMode(const String& screenModeStr) noexcept;

        enum class CursorMode
        {
            Default,
            Hidden,
            Disabled
        };

        struct WindowSettings
        {
            static constexpr auto DefaultX = 100;
            static constexpr auto DefaultY = 100;
            static constexpr auto DefaultWidth = 1920;
            static constexpr auto DefaultHeight = 1080;

            String name = "";
            Math::Point2I position = Math::Point2I(DefaultX, DefaultY);
            Math::Size2I size = Math::Size2I(DefaultWidth, DefaultHeight);
            Math::Size2I windowedSize = Math::Size2I(DefaultWidth, DefaultHeight);
            ScreenMode screenMode = ScreenMode::Windowed;
            CursorMode cursorMode = CursorMode::Default;
            Math::Point2I cursorPosition = Math::Point2I(0, 0);
            bool vSync = true;
            bool updateContinuously = true;
            bool alwaysOnTop = false;
            UInt32 dpi = 96;
            float dpiScale = 1.0f;

            KMP_NODISCARD KMP_API bool IsFullscreen() const noexcept;
            KMP_NODISCARD KMP_API bool IsWindowed() const noexcept;
            KMP_NODISCARD KMP_API bool IsWindowedFullscreen() const noexcept;

            KMP_API void SaveSettings(SettingsDocument& settings) const;
            KMP_API void LoadSettings(SettingsDocument& settings);
        };

    public:
        KMP_API explicit Window(WindowSettings& settings);
        virtual ~Window() = default;

        KMP_NODISCARD KMP_API const String& GetName() const noexcept;

        KMP_NODISCARD KMP_API virtual Math::Size2I GetSize() const = 0;
        KMP_NODISCARD KMP_API virtual Math::Size2I GetWindowedSize() const = 0;
        KMP_NODISCARD KMP_API virtual bool IsIconified() const = 0;

        KMP_NODISCARD KMP_API virtual UInt32 GetDPI() const = 0;
        KMP_NODISCARD KMP_API virtual float GetDPIScale() const = 0;

        KMP_API virtual void SetTitle(const char* title) = 0;
        KMP_API virtual void SetIcon(const Image& image) = 0;

        KMP_API virtual void SetPosition(int x, int y) = 0;
        KMP_NODISCARD KMP_API virtual Math::Point2I GetPosition() const = 0;
        KMP_API virtual void PositionAtCurrentScreenCenter() = 0;

        KMP_API virtual void SetShouldClose(bool close) = 0;
        KMP_NODISCARD KMP_API virtual bool ShouldClose() const = 0;

        KMP_API virtual void SetScreenMode(ScreenMode mode) = 0;
        KMP_NODISCARD KMP_API virtual ScreenMode GetScreenMode() const = 0;

        KMP_NODISCARD KMP_API bool IsFullscreen() const noexcept;
        KMP_NODISCARD KMP_API bool IsWindowed() const noexcept;
        KMP_NODISCARD KMP_API bool IsWindowedFullscreen() const noexcept;

        KMP_API virtual void SetCursor(const WindowCursor& cursor) const = 0;
        KMP_API virtual void SetCursorMode(CursorMode cursorMode) = 0;
        KMP_NODISCARD KMP_API virtual CursorMode GetCursorMode() const = 0;
        KMP_NODISCARD KMP_API virtual Math::Point2I GetCursorPosition() const = 0;

        KMP_API virtual void SetVSync(bool vSync) = 0;
        KMP_NODISCARD KMP_API virtual bool IsVSync() const = 0;

        KMP_API virtual void SetUpdatedContinuously(bool updatedContinuously) = 0;
        KMP_NODISCARD KMP_API virtual bool IsUpdatedContinuously() const = 0;

        KMP_API virtual void SetResizable(bool resizable) = 0;
        KMP_NODISCARD KMP_API virtual bool IsResizable() const = 0;

        KMP_API virtual void SetDecorated(bool decorated) = 0;
        KMP_NODISCARD KMP_API virtual bool IsDecorated() const = 0;

        KMP_API virtual void SetAlwaysOnTop(bool alwaysOnTop) = 0;
        KMP_NODISCARD KMP_API virtual bool IsAlwaysOnTop() const = 0;

        KMP_API virtual void FetchEvents() = 0;
        KMP_API virtual void SwapBuffers() const = 0;
        KMP_API virtual void MakeContextCurrent() = 0;
        KMP_API virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        KMP_NODISCARD KMP_API virtual NonNull<void*> GetImplPointer() const noexcept = 0;

    protected:
        WindowSettings& _settings;
    };
    //--------------------------------------------------------------------------
}