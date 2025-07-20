#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types.h"
#include "Kmplete/Event/event.h"
#include "Kmplete/Profile/profiler.h"

#include <functional>

namespace Kmplete
{
    class Window
    {
#if defined(KMP_PROFILE)
    private:
        UPtr<ProfilerTimer> _constructorProfilerTimer;
#endif

    public:
        using EventCallbackFn = std::function<void(Event&)>;

        constexpr static auto NameStr = "Name";
        constexpr static auto WidthStr = "Width";
        constexpr static auto HeightStr = "Height";
        constexpr static auto WindowedWidthStr = "WindowedWidth";
        constexpr static auto WindowedHeightStr = "WindowedHeight";
        constexpr static auto ScreenModeStr = "ScreenMode";
        constexpr static auto VSyncStr = "VSync";
        constexpr static auto UpdateContinuouslyStr = "UpdateContinuously";
        constexpr static auto ResizableStr = "Resizable";
        constexpr static auto DecoratedStr = "Decorated";

        constexpr static auto DefaultWidth = 1920;
        constexpr static auto DefaultHeight = 1080;

    public:
        enum Mode
        {
            FullscreenMode,
            WindowedFullscreenMode,
            WindowedMode
        };

        constexpr static auto FullscreenModeStr = "Fullscreen";
        constexpr static auto WindowedFullscreenModeStr = "WindowedFullscreen";
        constexpr static auto WindowedModeStr = "Windowed";

        struct WindowSettings
        {
            KMP_API explicit WindowSettings(const String& name) noexcept;

            const String name = "";
            int width = DefaultWidth;
            int height = DefaultHeight;
            int windowedWidth = DefaultWidth;
            int windowedHeight = DefaultHeight;
            Mode screenMode = WindowedMode;
            bool vSync = true;
            bool updateContinuously = true;
            bool resizable = true;
            bool decorated = true;
            float dpiScale = 1.0f;
        };

        KMP_NODISCARD static String ModeToString(Mode mode) noexcept;
        KMP_NODISCARD static Mode StringToMode(const String& modeStr) noexcept;

    public:
        KMP_DISABLE_COPY_MOVE(Window)

        KMP_API explicit Window(WindowSettings& settings);
        virtual ~Window() = default;

        KMP_NODISCARD KMP_API const String& GetName() const;

        KMP_NODISCARD KMP_API virtual std::pair<int, int> GetSize() const = 0; //TODO: make geometric struct instead of std pair
        KMP_NODISCARD KMP_API virtual std::pair<int, int> GetWindowedSize() const = 0;

        KMP_NODISCARD KMP_API virtual float GetDPIScale() const = 0;

        KMP_API virtual void SetTitle(const char* title) = 0;
        KMP_API virtual void SetIcon(const Path& path) = 0;

        KMP_API virtual void SetShouldClose(bool close) = 0;
        KMP_NODISCARD KMP_API virtual bool ShouldClose() const = 0;

        KMP_API virtual void SetScreenMode(Mode mode) = 0;
        KMP_NODISCARD KMP_API virtual Mode GetScreenMode() const = 0;

        KMP_API virtual void SetVSync(bool vSync) = 0;
        KMP_NODISCARD KMP_API virtual bool IsVSync() const = 0;

        KMP_API virtual void SetUpdatedContinuously(bool updatedContinuously) = 0;
        KMP_NODISCARD KMP_API virtual bool IsUpdatedContinuously() const = 0;

        KMP_API virtual void SetResizable(bool resizable) = 0;
        KMP_NODISCARD KMP_API virtual bool IsResizable() const = 0;

        KMP_API virtual void SetDecorated(bool decorated) = 0;
        KMP_NODISCARD KMP_API virtual bool IsDecorated() const = 0;

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