#pragma once

#include "Kmplete/Core/kmplete_api.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/pointers.h"

struct GLFWwindow;

namespace Kmplete
{
    class WindowGlfw : public Window
    {
    public:
        KMP_API WindowGlfw();
        KMP_API ~WindowGlfw() = default;

        KMP_NODISCARD KMP_API bool Initialize() override;
        KMP_API void Finalize() override;

        KMP_API void SetTitle(const std::string& title) override;
        KMP_API void SetIcon(const std::string& path) override;

        KMP_API void SetShouldClose(bool close) override;
        KMP_NODISCARD KMP_API bool ShouldClose() const override;

        KMP_API void SetScreenMode(Mode mode) override;
        KMP_NODISCARD KMP_API Mode GetScreenMode() const override;

        KMP_API void SetVSync(bool vSync) override;
        KMP_NODISCARD KMP_API bool IsVSync() const override;

        KMP_API void ProcessEvents() override;
        KMP_API void SwapBuffers() const override;
        KMP_API void MakeContextCurrent() override;
        KMP_API void SetEventCallback(const EventCallbackFn& callback) override;

        KMP_NODISCARD KMP_API void* GetImplPointer() const KMP_NOEXCEPT override;

        KMP_API void SaveSettings(const Ptr<Settings> settings) const override;
        KMP_API void LoadSettings(const Ptr<Settings> settings) override;

    private:
        struct UserData
        {
            bool updateContinuously = true;
            Mode screenMode = WindowedMode;
            int width = DefaultWidth;
            int height = DefaultHeight;
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