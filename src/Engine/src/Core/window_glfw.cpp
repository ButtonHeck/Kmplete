#include "Kmplete/Core/window_glfw.h"
#include "Kmplete/Core/log.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Event/window_event.h"
#include "Kmplete/Event/key_event.h"
#include "Kmplete/Event/mouse_event.h"
#include "Kmplete/Graphics/image.h"

#include <GLFW/glfw3.h>

#include <stdexcept>

namespace Kmplete
{
    WindowGlfw::WindowGlfw(const Ptr<WindowSettings> settings)
        : Window(settings)
        , _window(nullptr)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    WindowGlfw::~WindowGlfw()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::Initialize()
    {
        InitializeHints();

        _window = glfwCreateWindow(_settings->width, _settings->height, "", nullptr, nullptr);

        if (!_window)
        {
            Log::CoreError("WindowGlfw: creation failed");
            throw std::runtime_error("WindowGlfw creation failed");
        }

        glfwSetWindowUserPointer(_window, new UserData());
        MakeContextCurrent();

        InitializeCallbacks();

        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->windowedWidth = _settings->windowedWidth;
            userData->windowedHeight = _settings->windowedHeight;
            userData->updateContinuously = _settings->updateContinuously;
        }

        SetUpdatedContinuously(_settings->updateContinuously);
        SetVSync(_settings->vSync);
        SetScreenMode(Window::StringToMode(_settings->screenMode));

        if (Window::StringToMode(_settings->screenMode) == WindowedMode)
        {
            glfwSetWindowSize(_window, _settings->windowedWidth, _settings->windowedHeight);
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::Finalize()
    {
        UpdateSettings();
        glfwDestroyWindow(_window);
    }
    //--------------------------------------------------------------------------

    std::pair<int, int> WindowGlfw::GetSize() const
    {
        int width;
        int height;
        glfwGetWindowSize(_window, &width, &height);
        return std::pair<int, int>(width, height);
    }
    //--------------------------------------------------------------------------

    std::pair<int, int> WindowGlfw::GetWindowedSize() const
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return std::pair<int, int>(userData->windowedWidth, userData->windowedHeight);
        }

        return std::pair<int, int>(DefaultWidth, DefaultHeight);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetTitle(const std::string& title)
    {
        glfwSetWindowTitle(_window, title.c_str());
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetIcon(const std::filesystem::path& path)
    {
        Image img(path, Image::RGBAlpha);
        GLFWimage icon{ img.GetWidth(), img.GetHeight(), img.GetPixels() };

        if (icon.pixels)
        {
            glfwSetWindowIcon(_window, 1, &icon);
        }
        else
        {
            Log::CoreWarn("WindowGlfw: cannot set window icon");
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetShouldClose(bool close)
    {
        glfwSetWindowShouldClose(_window, close);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::ShouldClose() const
    {
        return glfwWindowShouldClose(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetScreenMode(Mode mode)
    {
        const auto userData = GetUserPointer(_window);
        if (!userData || userData->screenMode == mode)
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
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return userData->screenMode;
        }

        return WindowedMode;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetVSync(bool vSync)
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->vSync = vSync;
        }

        glfwSwapInterval(vSync ? 1 : 0);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsVSync() const
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return userData->vSync;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetUpdatedContinuously(bool updatedContinuously)
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->updateContinuously = updatedContinuously;
        }
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsUpdatedContinuously() const
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return userData->updateContinuously;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetResizable(bool resizable)
    {
        Log::CoreWarn("WindowGlfw: resizable setting will be applied after restart");
        _settings->resizable = resizable;
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsResizable() const
    {
        return _settings->resizable;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetDecorated(bool decorated)
    {
        Log::CoreWarn("WindowGlfw: decorated settings will be applied after restart");
        _settings->decorated = decorated;
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsDecorated() const
    {
        return _settings->decorated;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::ProcessEvents()
    {
        const auto userData = GetUserPointer(_window);
        if (userData->updateContinuously)
        {
            glfwPollEvents();
        }
        else
        {
            glfwWaitEvents();
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SwapBuffers() const
    {
        glfwSwapBuffers(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::MakeContextCurrent()
    {
        glfwMakeContextCurrent(_window);
    }
    //--------------------------------------------------------------------------

    void* WindowGlfw::GetImplPointer() const KMP_NOEXCEPT
    {
        return _window;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetEventCallback(const EventCallbackFn& callback)
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->eventCallback = callback;
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::UpdateSettings() const
    {
        if (!_window)
        {
            Log::CoreWarn("WindowGlfw: can't update settings due to nullptr GLFW window");
            return;
        }

        const auto [width, height] = GetSize();
        const auto [windowedWidth, windowedHeight] = GetWindowedSize();

        _settings->width = width;
        _settings->height = height;
        _settings->windowedWidth = windowedWidth;
        _settings->windowedHeight = windowedHeight;
        _settings->screenMode = ModeToString(GetScreenMode());
        _settings->vSync = IsVSync();
        _settings->updateContinuously = IsUpdatedContinuously();
    }
    //--------------------------------------------------------------------------

    WindowGlfw::UserData* WindowGlfw::GetUserPointer(GLFWwindow* window)
    {
        auto userData = glfwGetWindowUserPointer(window);
        KMP_ASSERT(userData);

        return reinterpret_cast<UserData*>(userData);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeHints() const
    {
        glfwWindowHint(GLFW_RESIZABLE, _settings->resizable ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, _settings->decorated ? GLFW_TRUE : GLFW_FALSE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeCallbacks() const
    {
        glfwSetErrorCallback([](int code, const char* description) {
            Log::CoreError("WindowGlfw: GLFW internal error '{}': {}", code, description);
            }
        );

        glfwSetWindowPosCallback(_window, [](GLFWwindow* window, int x, int y) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                WindowMoveEvent event(x, y);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                WindowFocusEvent event(focused);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowIconifyCallback(_window, [](GLFWwindow* window, int iconified) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                WindowIconifyEvent event(iconified);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
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
            }
        );

        glfwSetWindowRefreshCallback(_window, [](GLFWwindow* window) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                WindowFramebufferRefreshEvent event;
                userData->eventCallback(event);
            }

            glfwSwapBuffers(window);
            }
        );

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                WindowCloseEvent event;
                userData->eventCallback(event);
            }
            }
        );

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, KMP_MB_UNUSED int scancode, int action, int mods) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
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
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                KeyCharEvent event(codepoint);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
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
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                MouseScrollEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                userData->eventCallback(event);
            }
            }
        );

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            const auto userData = GetUserPointer(window);
            if (userData && userData->eventCallback)
            {
                MouseMoveEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------
}