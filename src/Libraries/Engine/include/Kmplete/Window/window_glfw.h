#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Profile/profiler_fwd.h"
#include "Kmplete/Log/log_class_macro.h"


struct GLFWwindow;
struct GLFWmonitor;


namespace Kmplete
{
    //! GLFW implementation of a window object
    class KMP_API WindowGlfw : public Window
    {
        KMP_LOG_CLASSNAME(WindowGlfw)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(WindowGlfw)

    public:
        explicit WindowGlfw(WindowSettings& settings, Graphics::GraphicsBackendType graphicsBackendType);
        ~WindowGlfw();

        KMP_NODISCARD Math::Size2I GetSize() const override;
        KMP_NODISCARD Math::Size2I GetWindowedSize() const override;
        KMP_NODISCARD Math::Size2I GetFramebufferSize() const override;

        KMP_NODISCARD float GetWindowAspectRatio() const override;
        KMP_NODISCARD float GetWindowFramebufferAspectRatio() const override;
        KMP_NODISCARD Math::Size2F GetFramebufferSizeToWindowSizeRatio() const override;

        KMP_NODISCARD bool IsIconified() const override;

        KMP_NODISCARD UInt32 GetDPI() const override;
        KMP_NODISCARD float GetDPIScale() const override;

        void SetTitle(const char* title) override;
        void SetIcon(const Graphics::Image& image) override;
        void SetSizeLimits(const Math::Size2I& minSize, const Math::Size2I& maxSize) override;

        void SetPosition(int x, int y) override;
        KMP_NODISCARD Math::Point2I GetPosition() const override;
        void PositionAtCurrentScreenCenter() override;

        void SetShouldClose(bool close) override;
        KMP_NODISCARD bool ShouldClose() const override;

        void SetScreenMode(ScreenMode screenMode) override;
        KMP_NODISCARD ScreenMode GetScreenMode() const override;

        void SetCursor(const WindowCursor& cursor) const override;
        void SetCursorMode(CursorMode cursorMode) override;
        KMP_NODISCARD CursorMode GetCursorMode() const override;
        KMP_NODISCARD Math::Point2I GetCursorPosition() const override;

        void SetUpdatedContinuously(bool updatedContinuously) override;
        KMP_NODISCARD bool IsUpdatedContinuously() const override;

        void SetResizable(bool resizable) override;
        KMP_NODISCARD bool IsResizable() const override;

        void SetDecorated(bool decorated) override;
        KMP_NODISCARD bool IsDecorated() const override;

        void SetAlwaysOnTop(bool alwaysOnTop) override;
        KMP_NODISCARD bool IsAlwaysOnTop() const override;

        void FetchEvents() override;
        void SwapBuffers() const override;
        void SetEventCallback(const EventCallbackFn& callback) override;

        KMP_NODISCARD NonNull<void*> GetImplPointer() const noexcept override;

    private:
        //! Helper window storage struct, injected to GLFWwindow as a user pointer
        struct _UserData
        {
            explicit _UserData(WindowSettings& settings);

            ScreenMode& screenMode;
            Math::Point2I& position;
            Math::Size2I& size;
            Math::Size2I& windowedSize;
            Math::Point2I& cursorPosition;
            UInt32& dpi;
            float& dpiScale;
            EventCallbackFn eventCallback;
            bool iconified;
        };

        KMP_NODISCARD static NonNull<_UserData*> _GetUserPointer(const NonNull<GLFWwindow*> window);
        KMP_NODISCARD static Pair<bool, NonNull<GLFWmonitor*>> _GetSuitableMonitor(const Math::Rect2I& windowRectangle);
        KMP_NODISCARD static Math::Rect2I _GetMonitorRectangle(const NonNull<GLFWmonitor*> monitor);
        static void _UpdateDPI(const NonNull<GLFWwindow*> window);
        static void _UpdateDPIScale(const NonNull<GLFWwindow*> window);

    private:
        KMP_NODISCARD Nullable<GLFWwindow*> _CreateGLFWwindow();

        void _Initialize();
        void _InitializeWindowHints() const;

        void _InitializeUserPointer() const;
        void _InitializeCallbacks() const;
        void _InitializeWindowCallbacks() const;
        void _InitializeKeyboardCallbacks() const;
        void _InitializeMouseCallbacks() const;
        void _InitializeGeometry();

        void _Finalize();

    private:
        GLFWwindow* _window;
    };
    //--------------------------------------------------------------------------
}