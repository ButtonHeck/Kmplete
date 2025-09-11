#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

#include <functional>

namespace Kmplete
{
    class WindowCursor;

    class Window
    {
        KMP_LOG_CLASSNAME(Window)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        using EventCallbackFn = std::function<void(Event&)>;

        constexpr static auto NameStr = "Name";
        constexpr static auto XStr = "X";
        constexpr static auto YStr = "Y";
        constexpr static auto WidthStr = "Width";
        constexpr static auto HeightStr = "Height";
        constexpr static auto WindowedWidthStr = "WindowedWidth";
        constexpr static auto WindowedHeightStr = "WindowedHeight";
        constexpr static auto ScreenModeStr = "ScreenMode";
        constexpr static auto VSyncStr = "VSync";
        constexpr static auto UpdateContinuouslyStr = "UpdateContinuously";
        constexpr static auto ResizableStr = "Resizable";
        constexpr static auto DecoratedStr = "Decorated";
        constexpr static auto AlwaysOnTopStr = "AlwaysOnTop";

        constexpr static auto DefaultX = 100;
        constexpr static auto DefaultY = 100;
        constexpr static auto DefaultWidth = 1920;
        constexpr static auto DefaultHeight = 1080;

    public:
        enum class ScreenMode
        {
            Fullscreen,
            WindowedFullscreen,
            Windowed
        };

        constexpr static auto FullscreenModeStr = "Fullscreen";
        constexpr static auto WindowedFullscreenModeStr = "WindowedFullscreen";
        constexpr static auto WindowedModeStr = "Windowed";

        KMP_NODISCARD static String ScreenModeToString(ScreenMode screenMode) noexcept;
        KMP_NODISCARD static ScreenMode StringToScreenMode(const String& screenModeStr) noexcept;

        enum class CursorMode
        {
            Default,
            Hidden,
            Disabled
        };

        struct WindowSettings
        {
            KMP_API explicit WindowSettings(const String& name) noexcept;

            const String name = "";
            Point2I position = Point2I(DefaultX, DefaultY);
            Size2I size = Size2I(DefaultWidth, DefaultHeight);
            Size2I windowedSize = Size2I(DefaultWidth, DefaultWidth);
            ScreenMode screenMode = ScreenMode::Windowed;
            CursorMode cursorMode = CursorMode::Default;
            bool vSync = true;
            bool updateContinuously = true;
            bool resizable = true;
            bool decorated = true;
            bool alwaysOnTop = false;
            float dpiScale = 1.0f;

            KMP_NODISCARD KMP_API bool IsFullscreen() const noexcept;
            KMP_NODISCARD KMP_API bool IsWindowed() const noexcept;
            KMP_NODISCARD KMP_API bool IsWindowedFullscreen() const noexcept;
        };

    public:
        KMP_DISABLE_COPY_MOVE(Window)

        KMP_API explicit Window(WindowSettings& settings);
        virtual ~Window() = default;

        KMP_NODISCARD KMP_API const String& GetName() const noexcept;

        KMP_NODISCARD KMP_API virtual Size2I GetSize() const = 0;
        KMP_NODISCARD KMP_API virtual Size2I GetWindowedSize() const = 0;

        KMP_NODISCARD KMP_API virtual float GetDPIScale() const = 0;

        KMP_API virtual void SetTitle(const char* title) = 0;
        KMP_API virtual void SetIcon(const Filepath& filepath) = 0;

        KMP_API virtual void SetPosition(int x, int y) = 0;
        KMP_NODISCARD KMP_API virtual Point2I GetPosition() const = 0;
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

        KMP_API virtual void ProcessEvents() = 0;
        KMP_API virtual void SwapBuffers() const = 0;
        KMP_API virtual void MakeContextCurrent() = 0;
        KMP_API virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        KMP_NODISCARD KMP_API virtual NonNull<void*> GetImplPointer() const noexcept = 0;

    protected:
        WindowSettings& _settings;
    };
    //--------------------------------------------------------------------------
}