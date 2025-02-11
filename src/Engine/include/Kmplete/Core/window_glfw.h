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
        KMP_DISABLE_COPY_MOVE(WindowGlfw)

        KMP_API WindowGlfw(const Ptr<WindowSettings> settings);
        KMP_API ~WindowGlfw();

        KMP_NODISCARD KMP_API std::pair<int, int> GetSize() const override;
        KMP_NODISCARD KMP_API std::pair<int, int> GetWindowedSize() const override;

        KMP_API void SetTitle(const std::string& title) override;
        KMP_API void SetIcon(const Path& path) override;

        KMP_API void SetShouldClose(bool close) override;
        KMP_NODISCARD KMP_API bool ShouldClose() const override;

        KMP_API void SetScreenMode(Mode mode) override;
        KMP_NODISCARD KMP_API Mode GetScreenMode() const override;

        KMP_API void SetVSync(bool vSync) override;
        KMP_NODISCARD KMP_API bool IsVSync() const override;

        KMP_API void SetUpdatedContinuously(bool updatedContinuously) override;
        KMP_NODISCARD KMP_API bool IsUpdatedContinuously() const override;

        KMP_API void SetResizable(bool resizable) override;
        KMP_NODISCARD KMP_API bool IsResizable() const override;

        KMP_API void SetDecorated(bool decorated) override;
        KMP_NODISCARD KMP_API bool IsDecorated() const override;

        KMP_API void ProcessEvents() override;
        KMP_API void SwapBuffers() const override;
        KMP_API void MakeContextCurrent() override;
        KMP_API void SetEventCallback(const EventCallbackFn& callback) override;
        KMP_API void UpdateSettings() const override;

        KMP_NODISCARD KMP_API NonNull<void*> GetImplPointer() const noexcept override;

    private:
        struct UserData
        {
            bool updateContinuously = true;
            bool vSync = true;
            Mode screenMode = WindowedMode;
            int width = DefaultWidth;
            int height = DefaultHeight;
            int windowedWidth = width;
            int windowedHeight = height;
            EventCallbackFn eventCallback;
        };

        KMP_NODISCARD static Nullable<UserData*> GetUserPointer(GLFWwindow* window);

    private:
        void Initialize();
        void InitializeHints() const;
        void InitializeCallbacks() const;
        void Finalize();

    private:
        GLFWwindow* _window;
    };
    //--------------------------------------------------------------------------
}