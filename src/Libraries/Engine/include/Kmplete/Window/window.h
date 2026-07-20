#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Base/functional.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Math/geometry.h"
#include "Kmplete/Graphics/graphics_base.h"
#include "Kmplete/Profile/profiler_fwd.h"
#include "Kmplete/Log/log_class_macro.h"


namespace Kmplete
{
    class WindowCursor;
    class SettingsDocument;

    namespace Graphics
    {
        class Image;
    }


    //! Base class for application window. Communication with the application is
    //! done via callback mechanism. Each WindowApplication-derived class may assume
    //! that it has an already created window named "Main".
    class KMP_API Window
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
            bool updateContinuously = true;
            bool alwaysOnTop = false;
            UInt32 dpi = 96;
            float dpiScale = 1.0f;

            KMP_NODISCARD bool IsFullscreen() const noexcept;
            KMP_NODISCARD bool IsWindowed() const noexcept;
            KMP_NODISCARD bool IsWindowedFullscreen() const noexcept;

            void SaveSettings(SettingsDocument& settings) const;
            void LoadSettings(SettingsDocument& settings);
        };

    public:
        Window(WindowSettings& settings, Graphics::GraphicsBackendType graphicsBackendType);
        virtual ~Window() = default;

        KMP_NODISCARD const String& GetName() const noexcept;
        KMP_NODISCARD Graphics::GraphicsBackendType GetGraphicsBackendType() const noexcept;

        KMP_NODISCARD virtual Math::Size2I GetSize() const = 0;
        KMP_NODISCARD virtual Math::Size2I GetWindowedSize() const = 0;
        KMP_NODISCARD virtual bool IsIconified() const = 0;

        KMP_NODISCARD virtual UInt32 GetDPI() const = 0;
        KMP_NODISCARD virtual float GetDPIScale() const = 0;

        virtual void SetTitle(const char* title) = 0;
        virtual void SetIcon(const Graphics::Image& image) = 0;
        virtual void SetSizeLimits(const Math::Size2I& minSize, const Math::Size2I& maxSize) = 0;

        virtual void SetPosition(int x, int y) = 0;
        KMP_NODISCARD virtual Math::Point2I GetPosition() const = 0;
        virtual void PositionAtCurrentScreenCenter() = 0;

        virtual void SetShouldClose(bool close) = 0;
        KMP_NODISCARD virtual bool ShouldClose() const = 0;

        virtual void SetScreenMode(ScreenMode mode) = 0;
        KMP_NODISCARD virtual ScreenMode GetScreenMode() const = 0;

        KMP_NODISCARD bool IsFullscreen() const noexcept;
        KMP_NODISCARD bool IsWindowed() const noexcept;
        KMP_NODISCARD bool IsWindowedFullscreen() const noexcept;

        virtual void SetCursor(const WindowCursor& cursor) const = 0;
        virtual void SetCursorMode(CursorMode cursorMode) = 0;
        KMP_NODISCARD virtual CursorMode GetCursorMode() const = 0;
        KMP_NODISCARD virtual Math::Point2I GetCursorPosition() const = 0;

        virtual void SetUpdatedContinuously(bool updatedContinuously) = 0;
        KMP_NODISCARD virtual bool IsUpdatedContinuously() const = 0;

        virtual void SetResizable(bool resizable) = 0;
        KMP_NODISCARD virtual bool IsResizable() const = 0;

        virtual void SetDecorated(bool decorated) = 0;
        KMP_NODISCARD virtual bool IsDecorated() const = 0;

        virtual void SetAlwaysOnTop(bool alwaysOnTop) = 0;
        KMP_NODISCARD virtual bool IsAlwaysOnTop() const = 0;

        virtual void FetchEvents() = 0;
        virtual void SwapBuffers() const = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        KMP_NODISCARD virtual NonNull<void*> GetImplPointer() const noexcept = 0;

    protected:
        WindowSettings& _settings;
        Graphics::GraphicsBackendType _graphicsBackendType;
    };
    //--------------------------------------------------------------------------
}