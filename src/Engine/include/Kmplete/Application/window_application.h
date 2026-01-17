#pragma once

#include "Kmplete/Application/application.h"
#include "Kmplete/Application/frame_listener.h"
#include "Kmplete/Application/frame_listener_manager.h"
#include "Kmplete/Window/window_backend.h"
#include "Kmplete/Graphics/graphics_backend.h"
#include "Kmplete/Assets/assets_manager.h"
#include "Kmplete/Profile/profiler.h"
#include "Kmplete/Log/log_class_macro.h"
#include "Kmplete/Time/clock.h"
#include "Kmplete/Base/nullability.h"
#include "Kmplete/Input/input_manager.h"


namespace Kmplete
{
    class Window;


    //! Parameters for Kmplete window application creation, 
    //! encapsulates base application parameters
    //! @see WindowApplication
    struct WindowApplicationParameters
    {
        const ApplicationParameters applicationParameters;
        const bool resizable;
    };
    //--------------------------------------------------------------------------


    //! Application class that supports window and all subsystems that somehow depends on
    //! a window existence or subsystems that don't make sense without a window.
    //! It encapsulates the high-level control flow of a main loop, actual logic steps
    //! implementation are handled by frame listeners. If instance of this class has successfully been created,
    //! than there exists the window named "Main".
    //! Client code may redefine behaviour when the application is about to close by reimplementing
    //! "ConfirmExit" function (e.g. some editor apps may show additional dialog window if
    //! some data is not saved). This class is responsible for connecting low-level events (KeyPressed, WindowMoved etc.),
    //! provided by the underlying window, to the subsystems responsible of delegating or/and further processing these events.
    //! By default graphics API is set to OpenGL
    //! @see Application
    //! @see WindowBackend
    //! @see Window
    //! @see FrameListenerManager
    class WindowApplication : public Application
    {
        KMP_LOG_CLASSNAME(WindowApplication)
        KMP_PROFILE_CONSTRUCTOR_DECLARE()
        KMP_DISABLE_COPY_MOVE(WindowApplication)

    public:
        KMP_API explicit WindowApplication(const WindowApplicationParameters& parameters);
        KMP_API virtual ~WindowApplication();

        KMP_API void Run() override;

        KMP_NODISCARD KMP_API virtual bool ConfirmExit();

    protected:
        void OnEvent(Events::Event& event);

    private:
        void _Initialize(const WindowApplicationParameters& parameters);
        void _Finalize();

        KMP_NODISCARD bool _RunFrameIteration(Window& window);
        void _ProcessEvents(Window& window, float frameTimestep);

        void _SaveSettings() const;
        void _LoadSettings();

    protected:
        UPtr<WindowBackend> _windowBackend;
        UPtr<GraphicsBackend> _graphicsBackend;
        UPtr<Input::InputManager> _inputManager;
        UPtr<Assets::AssetsManager> _assetsManager;
        UPtr<FrameListenerManager> _frameListenerManager;

    private:
        GraphicsBackendType _graphicsBackendType;
        Time::Clock _frameClock;
    };
    //--------------------------------------------------------------------------
}