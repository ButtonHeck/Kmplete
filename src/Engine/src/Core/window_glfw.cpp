#include "Kmplete/Core/window_glfw.h"
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

    void WindowGlfw::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        InitializeHints();

        {
            KMP_PROFILE_SCOPE("GLFW window creation");
            _window = glfwCreateWindow(_settings.width, _settings.height, "", nullptr, nullptr);
        }

        if (!_window)
        {
            KMP_LOG_ERROR("WindowGlfw: creation failed");
            throw std::runtime_error("WindowGlfw creation failed");
        }

        glfwSetWindowUserPointer(_window, new UserData(_settings));
        KMP_ASSERT(GetUserPointer(_window));

        MakeContextCurrent();
        InitializeCallbacks();
        SetVSync(_settings.vSync);
        SetScreenMode(_settings.screenMode);

        if (_settings.screenMode == WindowedMode)
        {
            glfwSetWindowSize(_window, _settings.windowedWidth, _settings.windowedHeight);
        }

        float scale = 1.0f;
        glfwGetWindowContentScale(_window, &scale, &scale);
        _settings.dpiScale = scale;
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
            KMP_LOG_WARN("WindowGlfw: cannot set window icon");
        }
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

    void WindowGlfw::SetScreenMode(Mode mode)
    {
        KMP_PROFILE_FUNCTION();

        const NonNull<UserData*> userData = GetUserPointer(_window);
        if (userData->screenMode == mode)
        {
            return;
        }

        userData->screenMode = mode;

        const auto monitor = glfwGetPrimaryMonitor();
        const auto videoMode = glfwGetVideoMode(monitor);
        const auto monitorWidth = videoMode->width;
        const auto monitorHeight = videoMode->height;

        if (mode == FullscreenMode)
        {
            glfwSetWindowMonitor(_window, monitor, 0, 0, monitorWidth, monitorHeight, videoMode->refreshRate);
        }
        else if (mode == WindowedFullscreenMode)
        {
            int workingAreaX;
            int workingAreaY;
            int workingAreaWidth;
            int workingAreaHeight;
            glfwGetMonitorWorkarea(monitor, &workingAreaX, &workingAreaY, &workingAreaWidth, &workingAreaHeight);
            glfwSetWindowMonitor(_window, nullptr, workingAreaX, workingAreaY, workingAreaWidth, workingAreaHeight, videoMode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(_window, nullptr,
                (monitorWidth - userData->windowedWidth) / 2,
                (monitorHeight - userData->windowedHeight) / 2,
                userData->windowedWidth, userData->windowedHeight, videoMode->refreshRate);
        }
    }
    //--------------------------------------------------------------------------

    Window::Mode WindowGlfw::GetScreenMode() const
    {
        const NonNull<UserData*> userData = GetUserPointer(_window);
        return userData->screenMode;
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
        KMP_LOG_WARN("WindowGlfw: resizable setting will be applied after restart");
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
        KMP_LOG_WARN("WindowGlfw: decorated settings will be applied after restart");
        _settings.decorated = decorated;
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsDecorated() const
    {
        return _settings.decorated;
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

    void WindowGlfw::InitializeHints() const
    {
        KMP_PROFILE_FUNCTION();

        glfwWindowHint(GLFW_RESIZABLE, _settings.resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, _settings.decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeCallbacks() const
    {
        KMP_PROFILE_FUNCTION();

        InitializeErrorCallback();
        InitializeWindowCallbacks();
        InitializeKeyboardCallbacks();
        InitializeMouseCallbacks();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeErrorCallback() const
    {
        glfwSetErrorCallback([](KMP_MB_UNUSED int code, KMP_MB_UNUSED const char* description) {
            KMP_LOG_ERROR("WindowGlfw: GLFW internal error '{}': {}", code, description);
            }
        );
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

            if (userData->screenMode == WindowedMode)
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
}