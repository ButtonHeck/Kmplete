#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Event/event.h"

#include <string>
#include <functional>

namespace Kmplete
{
    class Settings;

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        constexpr static auto WidthStr = "Width";
        constexpr static auto HeightStr = "Height";
        constexpr static auto WindowedWidthStr = "WindowedWidth";
        constexpr static auto WindowedHeightStr = "WindowedHeight";
        constexpr static auto ScreenModeStr = "ScreenMode";
        constexpr static auto VSyncStr = "VSync";
        constexpr static auto UpdateContinuouslyStr = "UpdateContinuously";

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

    public:
        KMP_API virtual ~Window() = default;

        KMP_API KMP_NODISCARD virtual bool Initialize() = 0;

        KMP_API virtual void SetTitle(const std::string& title) = 0;
        KMP_API virtual void SetIcon(const std::string& path) = 0;

        KMP_API virtual void SetShouldClose(bool close) = 0;
        KMP_API KMP_NODISCARD virtual bool ShouldClose() const = 0;

        KMP_API virtual void SetScreenMode(Mode mode) = 0;
        KMP_API KMP_NODISCARD virtual Mode GetScreenMode() const = 0;

        KMP_API virtual void SetVSync(bool vSync) = 0;
        KMP_API KMP_NODISCARD virtual bool IsVSync() const = 0;

        KMP_API virtual void ProcessEvents() = 0;
        KMP_API virtual void SwapBuffers() const = 0;
        KMP_API virtual void MakeContextCurrent() = 0;
        KMP_API virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        KMP_API KMP_NODISCARD virtual void* GetImplPointer() const KMP_NOEXCEPT = 0;

        KMP_API virtual void SaveSettings(const Ptr<Settings> settings) const = 0;
        KMP_API virtual void LoadSettings(const Ptr<Settings> settings) = 0;

    protected:
        KMP_NODISCARD static std::string ModeToString(Mode mode) KMP_NOEXCEPT;
        KMP_NODISCARD static Mode StringToMode(const std::string& modeStr) KMP_NOEXCEPT;

    protected:
        constexpr static auto _DefaultWidth = 1920;
        constexpr static auto _DefaultHeight = 1080;
    };
    //--------------------------------------------------------------------------
}