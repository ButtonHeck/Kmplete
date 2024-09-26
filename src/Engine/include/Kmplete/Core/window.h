#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/pointers.h"
#include "Kmplete/Event/event.h"

#include <string>
#include <functional>

namespace Kmplete
{
    class Window
    {
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
            KMP_API explicit WindowSettings(const std::string& name);

            std::string name = "";
            unsigned int width = DefaultWidth;
            unsigned int height = DefaultHeight;
            unsigned int windowedWidth = DefaultWidth;
            unsigned int windowedHeight = DefaultHeight;
            std::string screenMode = WindowedModeStr;
            bool vSync = true;
            bool updateContinuously = true;
        };

        KMP_NODISCARD static std::string ModeToString(Mode mode) KMP_NOEXCEPT;
        KMP_NODISCARD static Mode StringToMode(const std::string& modeStr) KMP_NOEXCEPT;

    public:
        KMP_DISABLE_COPY_MOVE(Window)

        KMP_API Window(const Ptr<WindowSettings> settings);
        KMP_API virtual ~Window() = default;

        KMP_NODISCARD KMP_API const std::string& GetName() const;

        KMP_NODISCARD KMP_API virtual std::pair<int, int> GetSize() const = 0; //TODO: make geometric struct instead of std pair
        KMP_NODISCARD KMP_API virtual std::pair<int, int> GetWindowedSize() const = 0;

        KMP_API virtual void SetTitle(const std::string& title) = 0;
        KMP_API virtual void SetIcon(const std::string& path) = 0;

        KMP_API virtual void SetShouldClose(bool close) = 0;
        KMP_NODISCARD KMP_API virtual bool ShouldClose() const = 0;

        KMP_API virtual void SetScreenMode(Mode mode) = 0;
        KMP_NODISCARD KMP_API virtual Mode GetScreenMode() const = 0;

        KMP_API virtual void SetVSync(bool vSync) = 0;
        KMP_NODISCARD KMP_API virtual bool IsVSync() const = 0;

        KMP_API virtual void SetUpdatedContinuously(bool updatedContinuously) = 0;
        KMP_NODISCARD KMP_API virtual bool IsUpdatedContinuously() const = 0;

        KMP_API virtual void ProcessEvents() = 0;
        KMP_API virtual void SwapBuffers() const = 0;
        KMP_API virtual void MakeContextCurrent() = 0;
        KMP_API virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        KMP_NODISCARD KMP_API virtual void* GetImplPointer() const KMP_NOEXCEPT = 0;

    protected:
        Ptr<WindowSettings> _settings;
    };
    //--------------------------------------------------------------------------
}