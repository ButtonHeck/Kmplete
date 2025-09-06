#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"

struct GLFWwindow;

namespace Kmplete
{
    class WindowGlfw : public Window
    {
        KMP_LOG_CLASSNAME(WindowGlfw)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()

    public:
        KMP_DISABLE_COPY_MOVE(WindowGlfw)

        KMP_API explicit WindowGlfw(WindowSettings& settings);
        KMP_API ~WindowGlfw();

        KMP_NODISCARD KMP_API std::pair<int, int> GetSize() const override;
        KMP_NODISCARD KMP_API std::pair<int, int> GetWindowedSize() const override;

        KMP_NODISCARD KMP_API float GetDPIScale() const override;

        KMP_API void SetTitle(const char* title) override;
        KMP_API void SetIcon(const Filepath& filepath) override;
        KMP_API void SetPosition(int x, int y) override;

        KMP_API void SetShouldClose(bool close) override;
        KMP_NODISCARD KMP_API bool ShouldClose() const override;

        KMP_API void SetScreenMode(ScreenMode screenMode) override;
        KMP_NODISCARD KMP_API ScreenMode GetScreenMode() const override;

        KMP_API void SetCursor(const WindowCursor& cursor) const override;
        KMP_API void SetCursorMode(CursorMode cursorMode) override;
        KMP_NODISCARD KMP_API CursorMode GetCursorMode() const override;

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

        KMP_NODISCARD KMP_API NonNull<void*> GetImplPointer() const noexcept override;

    private:
        struct UserData
        {
            explicit UserData(WindowSettings& settings);

            ScreenMode& screenMode;
            int& width;
            int& height;
            int& windowedWidth;
            int& windowedHeight;
            float& dpiScale;
            EventCallbackFn eventCallback;
        };

        KMP_NODISCARD static NonNull<UserData*> GetUserPointer(GLFWwindow* window);

    private:
        Nullable<GLFWwindow*> CreateGLFWwindow();

        void Initialize();
        void InitializeWindowHints() const;

        void InitializeUserPointer() const;
        void InitializeCallbacks() const;
        void InitializeWindowCallbacks() const;
        void InitializeKeyboardCallbacks() const;
        void InitializeMouseCallbacks() const;

        void InitializeSize();
        void InitializeDPIScale();

        void Finalize();

    private:
        GLFWwindow* _window;
    };
    //--------------------------------------------------------------------------
}