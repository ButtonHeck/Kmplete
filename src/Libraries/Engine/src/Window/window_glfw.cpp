#include "Kmplete/Window/window_glfw.h"
#include "Kmplete/Window/window_cursor_glfw.h"
#include "Kmplete/Base/named_bool.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Event/window_events.h"
#include "Kmplete/Event/key_events.h"
#include "Kmplete/Event/mouse_events.h"
#include "Kmplete/Graphics/image.h"
#include "Kmplete/Math/geometry.h"

#include <GLFW/glfw3.h>

#include <stdexcept>


namespace Kmplete
{
    NonNull<WindowGlfw::_UserData*> WindowGlfw::_GetUserPointer(const NonNull<GLFWwindow*> window)
    {
        auto userData = glfwGetWindowUserPointer(window);
        KMP_ASSERT(userData);

        return reinterpret_cast<_UserData*>(userData);
    }
    //--------------------------------------------------------------------------

    std::pair<bool, NonNull<GLFWmonitor*>> WindowGlfw::_GetSuitableMonitor(const Math::Rect2I& windowRectangle)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        const auto windowCenter = windowRectangle.GetCenter();

        int count = 0;
        const auto monitors = glfwGetMonitors(&count);

        if (count && monitors)
        {
            for (auto i = 0; i < count; i++)
            {
                const auto monitor = monitors[i];
                if (monitor)
                {
                    const auto monitorRectangle = _GetMonitorRectangle(monitor);
                    if (monitorRectangle.ContainsPoint(windowCenter))
                    {
                        return { "suitable"_true, monitor };
                    }
                }
            }
        }

        KMP_LOG_WARN("cannot get window's current monitor, primary monitor will be used");

        return { "suitable"_false, glfwGetPrimaryMonitor() };
    }
    //--------------------------------------------------------------------------

    Math::Rect2I WindowGlfw::_GetMonitorRectangle(const NonNull<GLFWmonitor*> monitor)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const auto videoMode = glfwGetVideoMode(monitor);
        const auto monitorScreenWidth = videoMode->width;
        const auto monitorScreenHeight = videoMode->height;
        KMP_ASSERT(monitorScreenWidth != 0 && monitorScreenHeight != 0);

        int monitorX = 0;
        int monitorY = 0;
        glfwGetMonitorPos(monitor, &monitorX, &monitorY);

        return Math::Rect2I(Math::Point2I(monitorX, monitorY), Math::Size2I(monitorScreenWidth, monitorScreenHeight));
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_UpdateDPI(const NonNull<GLFWwindow*> window)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const NonNull<_UserData*> userData = _GetUserPointer(window);

        const auto [isFound, monitor] = _GetSuitableMonitor(Math::Rect2I(userData->position, userData->size));
        const auto videoMode = glfwGetVideoMode(monitor);
        auto monitorWidthMm = 0;
        auto monitorHeightMm = 0;
        glfwGetMonitorPhysicalSize(monitor, &monitorWidthMm, &monitorHeightMm);
        const auto monitorDiagonalInch = glm::length(Math::Size2F(float(monitorWidthMm), float(monitorHeightMm))) / 25.4f;
        const auto monitorDiagonalPixels = glm::length(Math::Size2F(float(videoMode->width), float(videoMode->height)));
        if (monitorDiagonalInch == 0.0f)
        {
            KMP_LOG_WARN("failed to get monitor diagonal size");
            return;
        }

        userData->dpi = UInt32(monitorDiagonalPixels / monitorDiagonalInch);

        KMP_LOG_INFO("window DPI changed to {}", userData->dpi);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_UpdateDPIScale(const NonNull<GLFWwindow*> window)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const NonNull<_UserData*> userData = _GetUserPointer(window);

        auto scale = 1.0f;
        glfwGetWindowContentScale(window, &scale, &scale);
        userData->dpiScale = scale;
    }
    //--------------------------------------------------------------------------


    WindowGlfw::_UserData::_UserData(WindowSettings& settings)
        : screenMode(settings.screenMode)
        , position(settings.position)
        , size(settings.size)
        , windowedSize(settings.windowedSize)
        , cursorPosition(settings.cursorPosition)
        , dpi(settings.dpi)
        , dpiScale(settings.dpiScale)
        , iconified(false)
    {}
    //--------------------------------------------------------------------------


    WindowGlfw::WindowGlfw(WindowSettings& settings, Graphics::GraphicsBackendType graphicsBackendType)
        : Window(settings, graphicsBackendType)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _window(nullptr)
    {
        _Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    WindowGlfw::~WindowGlfw()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _Finalize();
    }
    //--------------------------------------------------------------------------

    Nullable<GLFWwindow*> WindowGlfw::_CreateGLFWwindow()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto [isFound, monitor] = _GetSuitableMonitor(Math::Rect2I(_settings.position, _settings.size));
        const auto videoMode = glfwGetVideoMode(monitor);

        GLFWwindow* window = nullptr;

        if (IsFullscreen())
        {
            _settings.size.x = videoMode->width;
            _settings.size.y = videoMode->height;
            window = glfwCreateWindow(_settings.size.x, _settings.size.y, "", monitor, nullptr);
        }
        else if (IsWindowedFullscreen())
        {
            _settings.size.x = videoMode->width;
            _settings.size.y = videoMode->height;
            window = glfwCreateWindow(_settings.size.x, _settings.size.y, "", nullptr, nullptr);
        }
        else
        {
            window = glfwCreateWindow(_settings.windowedSize.x, _settings.windowedSize.y, "", nullptr, nullptr);
        }

        return window;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_Initialize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _InitializeWindowHints();

        _window = _CreateGLFWwindow();

        if (!_window)
        {
            KMP_LOG_ERROR("creation failed");
            throw std::runtime_error("WindowGlfw creation failed");
        }

        _InitializeUserPointer();
        _InitializeGeometry();
        _InitializeCallbacks();

        _UpdateDPIScale(_window);
        _UpdateDPI(_window);

        glfwShowWindow(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        NonNull<_UserData*> userData = _GetUserPointer(_window);
        delete userData;

        glfwDestroyWindow(_window);
    }
    //--------------------------------------------------------------------------

    Math::Size2I WindowGlfw::GetSize() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        int width;
        int height;
        glfwGetWindowSize(_window, &width, &height);
        return Math::Size2I(width, height);
    }
    //--------------------------------------------------------------------------

    Math::Size2I WindowGlfw::GetWindowedSize() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const NonNull<_UserData*> userData = _GetUserPointer(_window);
        return Math::Size2I(userData->windowedSize.x, userData->windowedSize.y);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsIconified() const
    {
        const NonNull<_UserData*> userData = _GetUserPointer(_window);
        return userData->iconified;
    }
    //--------------------------------------------------------------------------

    UInt32 WindowGlfw::GetDPI() const
    {
        return _settings.dpi;
    }
    //--------------------------------------------------------------------------

    float WindowGlfw::GetDPIScale() const
    {
        return _settings.dpiScale;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetTitle(const char* title)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        glfwSetWindowTitle(_window, title);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetIcon(const Graphics::Image& image)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);
        KMP_ASSERT(image.GetPixels());

        GLFWimage icon{ image.GetWidth(), image.GetHeight(), image.GetPixels() };

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
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        glfwSetWindowPos(_window, x, y);
    }
    //--------------------------------------------------------------------------

    Math::Point2I WindowGlfw::GetPosition() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        int x;
        int y;
        glfwGetWindowPos(_window, &x, &y);
        return Math::Point2I(x, y);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::PositionAtCurrentScreenCenter()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctionsVerbose);

        if (!IsWindowed())
        {
            KMP_LOG_WARN("positioning at the current screen's center applied only to windowed screen mode");
            return;
        }

        const auto [isFound, currentMonitor] = _GetSuitableMonitor(Math::Rect2I(_settings.position, _settings.size));
        const auto monitorRectangle = _GetMonitorRectangle(currentMonitor);
        const auto monitorCenter = monitorRectangle.GetCenter();

        const auto windowedSize = GetWindowedSize();
        const NonNull<_UserData*> userData = _GetUserPointer(_window);

        glfwSetWindowMonitor(_window, nullptr,
            monitorCenter.x - windowedSize.x / 2,
            monitorCenter.y - windowedSize.y / 2,
            userData->windowedSize.x, userData->windowedSize.y, GLFW_DONT_CARE); 
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetShouldClose(bool close)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        glfwSetWindowShouldClose(_window, close);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::ShouldClose() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        return glfwWindowShouldClose(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetScreenMode(ScreenMode screenMode)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        const NonNull<_UserData*> userData = _GetUserPointer(_window);
        if (userData->screenMode == screenMode)
        {
            return;
        }

        userData->screenMode = screenMode;

        const auto [isFound, monitor] = _GetSuitableMonitor(Math::Rect2I(_settings.position, _settings.size));
        const auto videoMode = glfwGetVideoMode(monitor);
        const auto monitorRectangle = _GetMonitorRectangle(monitor);

        if (screenMode == ScreenMode::Fullscreen)
        {
            glfwSetWindowMonitor(_window, monitor, monitorRectangle.position.x, monitorRectangle.position.y, monitorRectangle.size.x, monitorRectangle.size.y, videoMode->refreshRate);
        }
        else if (screenMode == ScreenMode::WindowedFullscreen)
        {
            SetDecorated(false);
            glfwSetWindowMonitor(_window, nullptr, monitorRectangle.position.x, monitorRectangle.position.y, monitorRectangle.size.x, monitorRectangle.size.y, GLFW_DONT_CARE);
        }
        else
        {
            glfwHideWindow(_window);
            PositionAtCurrentScreenCenter();
            SetDecorated(true);
            glfwShowWindow(_window);
        }
    }
    //--------------------------------------------------------------------------

    Window::ScreenMode WindowGlfw::GetScreenMode() const
    {
        const NonNull<_UserData*> userData = _GetUserPointer(_window);
        return userData->screenMode;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetCursor(const WindowCursor& cursor) const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

        KMP_ASSERT(cursor.GetImplPointer());

        glfwSetCursor(_window, reinterpret_cast<GLFWcursor*>(cursor.GetImplPointer().get()));
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetCursorMode(CursorMode cursorMode)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

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

    Math::Point2I WindowGlfw::GetCursorPosition() const
    {
        return _settings.cursorPosition;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetVSync(bool vSync)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelMinorFunctions);

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
        glfwSetWindowAttrib(_window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsResizable() const
    {
        return glfwGetWindowAttrib(_window, GLFW_RESIZABLE) == GLFW_TRUE;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetDecorated(bool decorated)
    {
        glfwSetWindowAttrib(_window, GLFW_DECORATED, decorated ? GLFW_TRUE : GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsDecorated() const
    {
        return glfwGetWindowAttrib(_window, GLFW_DECORATED) == GLFW_TRUE;
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

    void WindowGlfw::FetchEvents()
    {
        if (_settings.updateContinuously)
        {
            KMP_PROFILE_SCOPE("glfwPollEvents", ProfileLevelAlways);
            glfwPollEvents();
        }
        else
        {
            KMP_PROFILE_SCOPE("glfwWaitEvents", ProfileLevelAlways);
            glfwWaitEvents();
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SwapBuffers() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        glfwSwapBuffers(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::MakeContextCurrent()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

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
        const NonNull<_UserData*> userData = _GetUserPointer(_window);
        userData->eventCallback = callback;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeWindowHints() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        if (_graphicsBackendType == Graphics::GraphicsBackendType::Vulkan)
        {
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, _settings.alwaysOnTop ? GLFW_TRUE : GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeUserPointer() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        glfwSetWindowUserPointer(_window, new _UserData(_settings));
        KMP_ASSERT(_GetUserPointer(_window));
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeCallbacks() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _InitializeWindowCallbacks();
        _InitializeKeyboardCallbacks();
        _InitializeMouseCallbacks();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeWindowCallbacks() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        glfwSetWindowPosCallback(_window, [](GLFWwindow* window, int x, int y) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            userData->position.x = x;
            userData->position.y = y;

            if (userData->eventCallback)
            {
                Events::WindowMoveEvent event(x, y);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowContentScaleCallback(_window, [](GLFWwindow* window, float xScale, float) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            
            _UpdateDPIScale(window);
            _UpdateDPI(window);

            if (userData->eventCallback)
            {
                Events::WindowContentScaleEvent event(xScale);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                Events::WindowFocusEvent event(focused);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowIconifyCallback(_window, [](GLFWwindow* window, int iconified) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            userData->iconified = (iconified == GLFW_TRUE);
            if (userData->eventCallback)
            {
                Events::WindowIconifyEvent event(iconified);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                Events::WindowResizeEvent event(width, height);
                userData->eventCallback(event);
            }
        });

        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            userData->size.x = width;
            userData->size.y = height;

            if (userData->screenMode == ScreenMode::Windowed)
            {
                userData->windowedSize.x = width;
                userData->windowedSize.y = height;
            }

            if (userData->eventCallback)
            {
                Events::WindowFramebufferResizeEvent event(width, height);
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowRefreshCallback(_window, [](GLFWwindow* window) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                Events::WindowFramebufferRefreshEvent event;
                userData->eventCallback(event);
            }
            }
        );

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                Events::WindowCloseEvent event;
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeKeyboardCallbacks() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, KMP_MB_UNUSED int scancode, int action, int mods) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                switch (action)
                {
                case GLFW_PRESS:
                {
                    Events::KeyPressEvent event(key, mods, "repeat"_false);
                    userData->eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Events::KeyReleaseEvent event(key, mods);
                    userData->eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    Events::KeyPressEvent event(key, mods, "repeat"_true);
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
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                Events::KeyCharEvent event(codepoint);
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeMouseCallbacks() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                switch (action)
                {
                case GLFW_PRESS:
                {
                    Events::MouseButtonPressEvent event(button, mods);
                    userData->eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    Events::MouseButtonReleaseEvent event(button, mods);
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
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            if (userData->eventCallback)
            {
                Events::MouseScrollEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
                userData->eventCallback(event);
            }
            }
        );

        glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
            const NonNull<_UserData*> userData = _GetUserPointer(window);
            userData->cursorPosition.x = int(xPos);
            userData->cursorPosition.y = int(yPos);

            if (userData->eventCallback)
            {
                Events::MouseMoveEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
                userData->eventCallback(event);
            }
            }
        );
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::_InitializeGeometry()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto [isFound, monitor] = _GetSuitableMonitor(Math::Rect2I(_settings.position, _settings.size));

        if (IsWindowedFullscreen())
        {
            const auto monitorRectangle = _GetMonitorRectangle(monitor);

            SetDecorated(false);
            glfwSetWindowMonitor(_window, nullptr, monitorRectangle.position.x, monitorRectangle.position.y, _settings.size.x, _settings.size.y, GLFW_DONT_CARE);
            SetPosition(monitorRectangle.position.x, monitorRectangle.position.y);
        }
        else if (IsWindowed())
        {
            glfwSetWindowMonitor(_window, nullptr, _settings.position.x, _settings.position.y, _settings.windowedSize.x, _settings.windowedSize.y, GLFW_DONT_CARE);

            if (isFound)
            {
                SetPosition(_settings.position.x, _settings.position.y);
                glfwSetWindowSize(_window, _settings.windowedSize.x, _settings.windowedSize.y);
            }
            else
            {
                PositionAtCurrentScreenCenter();
            }
        }
    }
    //--------------------------------------------------------------------------
}