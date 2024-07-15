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
        KMP_API KMP_NODISCARD static Window* Create();

    public:
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        enum Mode
        {
            FullscreenMode,
            WindowedFullscreenMode,
            WindowedMode
        };

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

        KMP_API virtual void SaveSettings(Ptr<Settings> settings) const = 0;
        KMP_API virtual void LoadSettings(Ptr<Settings> settings) = 0;
    };
    //--------------------------------------------------------------------------
}