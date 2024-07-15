#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/settings.h"
#include "Kmplete/Core/pointers.h"

struct GLFWwindow;

namespace Kmplete
{
    class WindowGlfw : public Window
    {
    public:
        KMP_API WindowGlfw();
        KMP_API ~WindowGlfw();

        KMP_API KMP_NODISCARD bool Initialize() override;

        KMP_API void SetTitle(const std::string& title) override;
        KMP_API void SetIcon(const std::string& path) override;

        KMP_API void SetShouldClose(bool close) override;
        KMP_API KMP_NODISCARD bool ShouldClose() const override;

        KMP_API void SetScreenMode(Mode mode) override;
        KMP_API KMP_NODISCARD Mode GetScreenMode() const override;

        KMP_API void SetVSync(bool vSync) override;
        KMP_API KMP_NODISCARD bool IsVSync() const override;

        KMP_API void ProcessEvents() override;
        KMP_API void SwapBuffers() const override;
        KMP_API void MakeContextCurrent() override;
        KMP_API void SetEventCallback(const EventCallbackFn& callback) override;

        KMP_API KMP_NODISCARD void* GetImplPointer() const KMP_NOEXCEPT override;

        KMP_API void SaveSettings(const Ptr<Settings> settings) const override;
        KMP_API void LoadSettings(const Ptr<Settings> settings) override;

    private:
        constexpr static auto _DefaultWidth = 1920;
        constexpr static auto _DefaultHeight = 1080;

        struct UserData
        {
            bool updateContinuously = true;
            Mode screenMode = WindowedMode;
            int width = _DefaultWidth;
            int height = _DefaultHeight;
            int windowedWidth = width;
            int windowedHeight = height;
            bool vSync = true;
            EventCallbackFn eventCallback;
        };

        KMP_NODISCARD static UserData* GetUserPointer(GLFWwindow* window);

    private:
        void InitializeHints() const;
        void InitializeCallbacks() const;

    private:
        GLFWwindow* _window;
    };
    //--------------------------------------------------------------------------
}