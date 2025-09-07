#include "Kmplete/Window/window_glfw.h"
#include "Kmplete/Window/window_cursor_glfw.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Graphics/image.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Kmplete
{
    WindowGlfw::UserData::UserData(WindowSettings& settings)
        : screenMode(settings.screenMode)
        , width(settings.width)
        , height(settings.height)
        , windowedWidth(settings.windowedWidth)
        , windowedHeight(settings.windowedHeight)
        , dpiScale(settings.dpiScale)
    {}
    //--------------------------------------------------------------------------

    WindowGlfw::WindowGlfw(WindowSettings& settings)
        : Window(settings)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("WindowGlfw::WindowGlfw(WindowSettings&)")
        , _window(nullptr)
    {
        Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    WindowGlfw::~WindowGlfw()
    {
        KMP_PROFILE_FUNCTION();

        Finalize();
    }
    //--------------------------------------------------------------------------

    Nullable<GLFWwindow*> WindowGlfw::CreateGLFWwindow()
    {
        KMP_PROFILE_FUNCTION();

        const auto monitor = glfwGetPrimaryMonitor();
        const auto videoMode = glfwGetVideoMode(monitor);

        GLFWwindow* window = nullptr;

        if (IsFullscreen())
        {
            _settings.width = videoMode->width;
            _settings.height = videoMode->height;
            window = glfwCreateWindow(_settings.width, _settings.height, "", monitor, nullptr);
        }
        else if (IsWindowedFullscreen())
        {
            _settings.width = videoMode->width;
            _settings.height = videoMode->height;
            window = glfwCreateWindow(_settings.width, _settings.height, "", nullptr, nullptr);
        }
        else
        {
            window = glfwCreateWindow(_settings.width, _settings.height, "", nullptr, nullptr);
        }

        return window;
    }
    //--------------------------------------------------------------------------

    NonNull<GLFWmonitor*> WindowGlfw::GetCurrentMonitor() const
    {
        KMP_PROFILE_FUNCTION();

        const auto windowSize = GetSize();
        const auto windowPosition = GetPosition();

        int count = 0;
        const auto monitors = glfwGetMonitors(&count);

        if (count && monitors)
        {
            for (auto i = 0; i < count; i++)
            {
                const auto monitor = monitors[i];
                if (monitor)
                {
                    const auto videoMode = glfwGetVideoMode(monitor);
                    const auto monitorScreenWidth = videoMode->width;
                    const auto monitorScreenHeight = videoMode->height;
                    KMP_ASSERT(monitorScreenWidth != 0 && monitorScreenHeight != 0);

                    int monitorX = 0;
                    int monitorY = 0;
                    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

                    // TODO: logically it's just a check if a point is inside a rectangle, maybe extract to some math function in a future
                    const auto windowCenterX = windowPosition.first + (windowSize.first / 2);
                    const auto windowCenterY = windowPosition.second + (windowSize.second / 2);
                    const auto isInsideX = (windowCenterX >= monitorX && windowCenterX <= (monitorX + monitorScreenWidth));
                    const auto isInsideY = (windowCenterY >= monitorY && windowCenterY <= (monitorY + monitorScreenHeight));
                    if (isInsideX && isInsideY)
                    {
                        return monitor;
                    }
                }
            }
        }

        KMP_LOG_WARN("cannot get window's current monitor, primary monitor will be used");

        return glfwGetPrimaryMonitor();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        InitializeWindowHints();

        _window = CreateGLFWwindow();

        if (!_window)
        {
            KMP_LOG_ERROR("creation failed");
            throw std::runtime_error("WindowGlfw creation failed");
        }

        MakeContextCurrent();

        InitializeUserPointer();
        InitializeCallbacks();
        InitializeSize();
        InitializeDPIScale();

        SetVSync(_settings.vSync);

        glfwShowWindow(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        NonNull<UserData*> userData = GetUserPointer(_window);
        delete userData;

        glfwDestroyWindow(_window);
    }
    //--------------------------------------------------------------------------

    std::pair<int, int> WindowGlfw::GetSize() const
    {
        KMP_PROFILE_FUNCTION();

        int width;
        int height;
        glfwGetWindowSize(_window, &width, &height);
        return std::pair<int, int>(width, height);
    }
    //--------------------------------------------------------------------------

    std::pair<int, int> WindowGlfw::GetWindowedSize() const
    {
        KMP_PROFILE_FUNCTION();

        const NonNull<UserData*> userData = GetUserPointer(_window);
        return std::pair<int, int>(userData->windowedWidth, userData->windowedHeight);
    }
    //--------------------------------------------------------------------------

    float WindowGlfw::GetDPIScale() const
    {
        return _settings.dpiScale;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetTitle(const char* title)
    {
        KMP_PROFILE_FUNCTION();

        glfwSetWindowTitle(_window, title);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetIcon(const Filepath& filepath)
    {
        KMP_PROFILE_FUNCTION();

        Image img(filepath, ImageChannels::RGBAlpha);
        GLFWimage icon{ img.GetWidth(), img.GetHeight(), img.GetPixels() };

        if (icon.pixels)
        {
            glfwSetWindowIcon(_window, 1, &icon);
        }
        else
        {
            KMP_LOG_WARN("cannot set window icon");
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetPosition(int x, int y)
    {
        KMP_PROFILE_FUNCTION();

        glfwSetWindowPos(_window, x, y);
    }
    //--------------------------------------------------------------------------

    std::pair<int, int> WindowGlfw::GetPosition() const
    {
        KMP_PROFILE_FUNCTION();

        int x;
        int y;
        glfwGetWindowPos(_window, &x, &y);
        return std::pair<int, int>(x, y);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetShouldClose(bool close)
    {
        KMP_PROFILE_FUNCTION();

        glfwSetWindowShouldClose(_window, close);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::ShouldClose() const
    {
        KMP_PROFILE_FUNCTION();

        return glfwWindowShouldClose(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetScreenMode(ScreenMode screenMode)
    {
        KMP_PROFILE_FUNCTION();

        const NonNull<UserData*> userData = GetUserPointer(_window);
        if (userData->screenMode == screenMode)
        {
            return;
        }

        userData->screenMode = screenMode;

        const auto monitor = GetCurrentMonitor();
        const auto videoMode = glfwGetVideoMode(monitor);
        int monitorX = 0;
        int monitorY = 0;
        glfwGetMonitorPos(monitor, &monitorX, &monitorY);

        if (screenMode == ScreenMode::Fullscreen)
        {
            glfwSetWindowMonitor(_window, monitor, monitorX, monitorY, videoMode->width, videoMode->height, videoMode->refreshRate);
        }
        else if (screenMode == ScreenMode::WindowedFullscreen)
        {
            SetDecorated(false);
            glfwSetWindowMonitor(_window, nullptr, monitorX, monitorY, videoMode->width, videoMode->height, videoMode->refreshRate);
        }
        else
        {
            const auto windowedWidth = userData->windowedWidth;
            const auto windowedHeight = userData->windowedHeight;

            SetDecorated(true);

            userData->windowedWidth = windowedWidth;
            userData->windowedHeight = windowedHeight;

            glfwSetWindowMonitor(_window, nullptr,
                monitorX + (videoMode->width - userData->windowedWidth) / 2,
                monitorY + (videoMode->height - userData->windowedHeight) / 2,
                userData->windowedWidth, userData->windowedHeight, videoMode->refreshRate);
        }
    }
    //--------------------------------------------------------------------------

    Window::ScreenMode WindowGlfw::GetScreenMode() const
    {
        const NonNull<UserData*> userData = GetUserPointer(_window);
        return userData->screenMode;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetCursor(const WindowCursor& cursor) const
    {
        KMP_PROFILE_FUNCTION();

        KMP_ASSERT(cursor.GetImplPointer());

        glfwSetCursor(_window, reinterpret_cast<GLFWcursor*>(cursor.GetImplPointer()));
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetCursorMode(CursorMode cursorMode)
    {
        KMP_PROFILE_FUNCTION();

        _settings.cursorMode = cursorMode;
        if (cursorMode == CursorMode::Default)
        {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else if (cursorMode == CursorMode::Hidden)
        {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
        }
        else if (cursorMode == CursorMode::Disabled)
        {
            glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    }
    //--------------------------------------------------------------------------

    Window::CursorMode WindowGlfw::GetCursorMode() const
    {
        return _settings.cursorMode;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetVSync(bool vSync)
    {
        KMP_PROFILE_FUNCTION();

        _settings.vSync = vSync;
        glfwSwapInterval(vSync ? 1 : 0);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsVSync() const
    {
        return _settings.vSync;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetUpdatedContinuously(bool updatedContinuously)
    {
        _settings.updateContinuously = updatedContinuously;
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsUpdatedContinuously() const
    {
        return _settings.updateContinuously;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetResizable(bool resizable)
    {
        KMP_LOG_WARN("resizable setting will be applied after restart");
        _settings.resizable = resizable;
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsResizable() const
    {
        return _settings.resizable;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetDecorated(bool decorated)
    {
        _settings.decorated = decorated;
        glfwSetWindowAttrib(_window, GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsDecorated() const
    {
        return _settings.decorated;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetAlwaysOnTop(bool alwaysOnTop)
    {
        _settings.alwaysOnTop = alwaysOnTop;
        glfwSetWindowAttrib(_window, GLFW_FLOATING, alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsAlwaysOnTop() const
    {
        return _settings.alwaysOnTop;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::ProcessEvents()
    {
        if (_settings.updateContinuously)
        {
            KMP_PROFILE_SCOPE("glfwPollEvents");
            glfwPollEvents();
        }
        else
        {
            KMP_PROFILE_SCOPE("glfwWaitEvents");
            glfwWaitEvents();
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SwapBuffers() const
    {
        KMP_PROFILE_FUNCTION();

        glfwSwapBuffers(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::MakeContextCurrent()
    {
        KMP_PROFILE_FUNCTION();

        glfwMakeContextCurrent(_window);
    }
    //--------------------------------------------------------------------------

    NonNull<void*> WindowGlfw::GetImplPointer() const noexcept
    {
        return _window;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetEventCallback(const EventCallbackFn& callback)
    {
        const NonNull<UserData*> userData = GetUserPointer(_window);
        userData->eventCallback = callback;
    }
    //--------------------------------------------------------------------------

    NonNull<WindowGlfw::UserData*> WindowGlfw::GetUserPointer(GLFWwindow* window)
    {
        auto userData = glfwGetWindowUserPointer(window);
        KMP_ASSERT(userData);

        return reinterpret_cast<UserData*>(userData);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeWindowHints() const
    {
        KMP_PROFILE_FUNCTION();

        glfwWindowHint(GLFW_RESIZABLE, _settings.resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, _settings.decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, _settings.alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeUserPointer() const
    {
        KMP_PROFILE_FUNCTION();

        glfwSetWindowUserPointer(_window, new UserData(_settings));
        KMP_ASSERT(GetUserPointer(_window));
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeCallbacks() const
    {
        KMP_PROFILE_FUNCTION();

        InitializeWindowCallbacks();
        InitializeKeyboardCallbacks();
        InitializeMouseCallbacks();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeWindowCallbacks() const
    {
        glfwSetWindowPosCallback(_window, [](GLFWwindow* window, int x, int y) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                WindowMoveEvent event(x, y);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowContentScaleCallback(_window, [](GLFWwindow* window, float xScale, float) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            userData->dpiScale = xScale;

            if (userData->eventCallback)
            {
                WindowContentScaleEvent event(xScale);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                WindowFocusEvent event(focused);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowIconifyCallback(_window, [](GLFWwindow* window, int iconified) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                WindowIconifyEvent event(iconified);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            userData->width = width;
            userData->height = height;

            if (userData->screenMode == ScreenMode::Windowed)
            {
                userData->windowedWidth = width;
                userData->windowedHeight = height;
            }

            if (userData->eventCallback)
            {
                WindowFramebufferResizeEvent event(width, height);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowRefreshCallback(_window, [](GLFWwindow* window) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                WindowFramebufferRefreshEvent event;
                userData->eventCallback(event);
            }

            glfwSwapBuffers(window);
            }
        );

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                WindowCloseEvent event;
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeKeyboardCallbacks() const
    {
        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, KMP_MB_UNUSED int scancode, int action, int mods) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                switch (action)
                {
                case GLFW_PRESS:
                {
                    KeyPressEvent event(key, mods, false);
                    userData->eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleaseEvent event(key);
                    userData->eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressEvent event(key, mods, true);
                    userData->eventCallback(event);
                    break;
                }
                default:
                    break;
                }
            }
            }
        );

        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int codepoint) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                KeyCharEvent event(codepoint);
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeMouseCallbacks() const
    {
        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                switch (action)
                {
                case GLFW_PRESS:
                {
                    MouseButtonPressEvent event(button, mods);
                    userData->eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleaseEvent event(button, mods);
                    userData->eventCallback(event);
                    break;
                }
                default:
                    break;
                }
            }
            }
        );

        glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                MouseScrollEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                userData->eventCallback(event);
            }
            }
        );

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            const NonNull<UserData*> userData = GetUserPointer(window);
            if (userData->eventCallback)
            {
                MouseMoveEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeSize()
    {
        KMP_PROFILE_FUNCTION();

        if (IsWindowedFullscreen())
        {
            const auto monitor = glfwGetPrimaryMonitor();
            const auto videoMode = glfwGetVideoMode(monitor);

            SetDecorated(false);
            glfwSetWindowMonitor(_window, nullptr, 0, 0, _settings.width, _settings.height, videoMode->refreshRate);
        }
        else if (IsWindowed())
        {
            glfwSetWindowSize(_window, _settings.windowedWidth, _settings.windowedHeight);
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeDPIScale()
    {
        KMP_PROFILE_FUNCTION();

        auto scale = 1.0f;
        glfwGetWindowContentScale(_window, &scale, &scale);
        _settings.dpiScale = scale;
    }
    //--------------------------------------------------------------------------
}