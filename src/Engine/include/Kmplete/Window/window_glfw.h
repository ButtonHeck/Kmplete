#pragma once

#include "Kmplete/Base/kmplete_api.h"
#include "Kmplete/Base/pointers.h"
#include "Kmplete/Base/types_aliases.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"


struct GLFWwindow;
struct GLFWmonitor;


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

        KMP_NODISCARD KMP_API Math::Size2I GetSize() const override;
        KMP_NODISCARD KMP_API Math::Size2I GetWindowedSize() const override;
        KMP_NODISCARD KMP_API bool IsIconified() const override;

        KMP_NODISCARD KMP_API UInt32 GetDPI() const override;
        KMP_NODISCARD KMP_API float GetDPIScale() const override;

        KMP_API void SetTitle(const char* title) override;
        KMP_API void SetIcon(const Image& image) override;

        KMP_API void SetPosition(int x, int y) override;
        KMP_NODISCARD KMP_API Math::Point2I GetPosition() const override;
        KMP_API void PositionAtCurrentScreenCenter() override;

        KMP_API void SetShouldClose(bool close) override;
        KMP_NODISCARD KMP_API bool ShouldClose() const override;

        KMP_API void SetScreenMode(ScreenMode screenMode) override;
        KMP_NODISCARD KMP_API ScreenMode GetScreenMode() const override;

        KMP_API void SetCursor(const WindowCursor& cursor) const override;
        KMP_API void SetCursorMode(CursorMode cursorMode) override;
        KMP_NODISCARD KMP_API CursorMode GetCursorMode() const override;
        KMP_NODISCARD KMP_API Math::Point2I GetCursorPosition() const override;

        KMP_API void SetVSync(bool vSync) override;
        KMP_NODISCARD KMP_API bool IsVSync() const override;

        KMP_API void SetUpdatedContinuously(bool updatedContinuously) override;
        KMP_NODISCARD KMP_API bool IsUpdatedContinuously() const override;

        KMP_API void SetResizable(bool resizable) override;
        KMP_NODISCARD KMP_API bool IsResizable() const override;

        KMP_API void SetDecorated(bool decorated) override;
        KMP_NODISCARD KMP_API bool IsDecorated() const override;

        KMP_API void SetAlwaysOnTop(bool alwaysOnTop) override;
        KMP_NODISCARD KMP_API bool IsAlwaysOnTop() const override;

        KMP_API void ProcessEvents() override;
        KMP_API void SwapBuffers() const override;
        KMP_API void MakeContextCurrent() override;
        KMP_API void SetEventCallback(const EventCallbackFn& callback) override;

        KMP_NODISCARD KMP_API NonNull<void*> GetImplPointer() const noexcept override;

    private:
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
        KMP_NODISCARD static std::pair<bool, NonNull<GLFWmonitor*>> _GetSuitableMonitor(const Math::Rect2I& windowRectangle);
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