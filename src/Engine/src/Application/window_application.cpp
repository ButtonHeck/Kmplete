#include "Kmplete/Application/window_application.h"
#include "Kmplete/Internal/default_window_icon_data.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Filesystem/filesystem.h"
#include "Kmplete/Event/key_events.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "WindowApplication";
    static constexpr auto GraphicsBackendTypeStr = "GraphicsBackendType";


    WindowApplication::WindowApplication(const WindowApplicationParameters& parameters)
        : Application(parameters.applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _windowBackend(nullptr)
        , _graphicsBackend(nullptr)
        , _inputManager(nullptr)
        , _assetsManager(nullptr)
        , _frameListenerManager(nullptr)
        , _graphicsBackendType(GraphicsBackendType::OpenGL)
        , _frameClock()
    {
        _Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Run()
    {
        KMP_LOG_INFO_FN("'{}' main loop started...", _applicationName);

        _running = true;

        auto& mainWindow = _windowBackend->GetMainWindow();

        while (_running)
        {
            if (!_RunFrameIteration(mainWindow))
            {
                break;
            }
        }

        KMP_LOG_INFO_FN("'{}' main loop finished", _applicationName);
    }
    //--------------------------------------------------------------------------

    bool WindowApplication::ConfirmExit()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::OnEvent(Events::Event& event)
    {
        _inputManager->ProcessInputEvents(event);
        _frameListenerManager->_DispatchEventToFrameListeners(event);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::_Initialize(const WindowApplicationParameters& parameters)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);

        _LoadSettings();

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(defaultWindowIcon);
        mainWindow.SetResizable(parameters.resizable);
        mainWindow.SetEventCallback(KMP_BIND(WindowApplication::OnEvent));

        _graphicsBackend = GraphicsBackend::Create(_graphicsBackendType);
        KMP_ASSERT(_graphicsBackend);

        _inputManager = CreateUPtr<Input::InputManager>();
        KMP_ASSERT(_inputManager);

        _assetsManager = CreateUPtr<Assets::AssetsManager>(_applicationPath, _graphicsBackend->GetType());
        KMP_ASSERT(_assetsManager);

        _frameListenerManager = CreateUPtr<FrameListenerManager>();
        KMP_ASSERT(_frameListenerManager);

#if defined KMP_PROFILE
        _inputManager->MapInputToCallback(Input::Code::Key_F11, "switch_profiler_activity"_sid, [this](Input::InputControlValue value) {
            if (value == Input::ButtonPressedValue && _inputManager->GetKeyModifiersMask() & Input::Modifier::Alt)
            {
                const auto isProfilerActive = Profiler::Get().IsActive();
                Profiler::Get().SetActive(!isProfilerActive);
            }
            return true;
        });
#endif

        _frameClock.Mark();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::_Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _SaveSettings();

        _frameListenerManager.reset();
        _assetsManager.reset();
        _inputManager.reset();
        _graphicsBackend.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    bool WindowApplication::_RunFrameIteration(Window& window)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto frameTimestep = Math::Clamp(_frameClock.Mark(), 0.0f, 100.0f);

        _ProcessEvents(window);

        if (window.ShouldClose())
        {
            if (ConfirmExit())
            {
                _running = false;
                return false;
            }
            else
            {
                window.SetShouldClose(false);
            }
        }

        const auto mainWindowIsIconified = window.IsIconified();

        _frameListenerManager->_UpdateFrameListeners(frameTimestep, mainWindowIsIconified);

        if (!mainWindowIsIconified)
        {
            _frameListenerManager->_RenderFrameListeners();
        }

        _frameListenerManager->_ProcessFrameListenersCommands();

        window.SwapBuffers();

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::_ProcessEvents(Window& window)
    {
        _inputManager->ResetMouseMove();
        window.FetchEvents();
        _inputManager->PropagateActionEvents();
        _frameListenerManager->_DispatchQueuedEventsToFrameListeners();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::_SaveSettings() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }
        
        settings->get().SaveString(GraphicsBackendTypeStr, GraphicsBackendTypeToString(_graphicsBackendType));
        _windowBackend->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::_LoadSettings()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get setting entry for loading");
            return;
        }

        _graphicsBackendType = StringToGraphicsBackendType(settings->get().GetString(GraphicsBackendTypeStr, DefaultAPIStr));
        _windowBackend->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}
