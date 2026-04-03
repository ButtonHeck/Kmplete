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
#include "Kmplete/Event/window_events.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "WindowApplication";
    static constexpr auto IconifiedFPSStr = "IconifiedFPS";
    static constexpr auto IconifiedFPSMin = UInt32(10);
    static constexpr auto IconifiedFPSMax = UInt32(60);


    WindowApplication::WindowApplication(const WindowApplicationParameters& parameters)
        : Application(parameters.applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _windowBackend(nullptr)
        , _graphicsBackend(nullptr)
        , _inputManager(nullptr)
        , _assetsManager(nullptr)
        , _frameListenerManager(nullptr)
        , _frameClock()
        , _iconifiedFPS(10)
    {
        _Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication() KMP_PROFILING(ProfileLevelAlways)
    {
        _Finalize();
    }}
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

        if (event.GetTypeID() == Events::WindowFramebufferResizeEventTypeID)
        {
            _graphicsBackend->HandleWindowResize();
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::_Initialize(const WindowApplicationParameters& parameters) KMP_PROFILING(ProfileLevelAlways)
    {
        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get settings entry for loading");
        }

        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);
        if (settings.has_value())
        {
            _LoadWindowBackendSettings(*settings);
        }

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(defaultWindowIcon);
        mainWindow.SetResizable(parameters.resizable);
        mainWindow.SetEventCallback(KMP_BIND(WindowApplication::OnEvent));

        _graphicsBackend = Graphics::GraphicsBackend::Create(mainWindow);
        KMP_ASSERT(_graphicsBackend);
        if (settings.has_value())
        {
            _LoadGraphicsBackendSettings(*settings);
        }

        _inputManager = CreateUPtr<Input::InputManager>();
        KMP_ASSERT(_inputManager);

        _assetsManager = CreateUPtr<Assets::AssetsManager>(_applicationPath, *_graphicsBackend.get());
        KMP_ASSERT(_assetsManager);

        _frameListenerManager = CreateUPtr<FrameListenerManager>();
        KMP_ASSERT(_frameListenerManager);

#if defined KMP_PROFILE
        _inputManager->MapInputToCallback({Input::Code::Key_F11, {Input::ButtonPressedValue, Input::Modifier::Alt}}, "switch_profiler_activity"_sid, [this](Input::InputControlValue) {
            const auto isProfilerActive = Profiler::Get().IsActive();
            Profiler::Get().SetActive(!isProfilerActive);
            return true;
        });
#endif

        if (settings.has_value())
        {
            _LoadSettings(*settings);
        }

        _frameClock.Mark();
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_Finalize() KMP_PROFILING(ProfileLevelAlways)
    {
        _SaveSettings();

        _frameListenerManager.reset();
        _assetsManager.reset();
        _inputManager.reset();
        _graphicsBackend.reset();
        _windowBackend.reset();
    }}
    //--------------------------------------------------------------------------

    bool WindowApplication::_RunFrameIteration(Window& window) KMP_PROFILING(ProfileLevelAlways)
    {
        const auto frameTimestep = Math::Clamp(_frameClock.Mark(), 0.0f, 100.0f);

        _ProcessEvents(window, frameTimestep);

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

        if (mainWindowIsIconified)
        {
            _frameListenerManager->_ProcessFrameListenersCommands();
            _IconifiedSleep();
        }
        else
        {
            _graphicsBackend->StartFrame(frameTimestep);
            _frameListenerManager->_RenderFrameListeners();
            _frameListenerManager->_ProcessFrameListenersCommands();
            _graphicsBackend->EndFrame();
        }

        return true;
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_ProcessEvents(Window& window, float frameTimestep) KMP_PROFILING(ProfileLevelImportant)
    {
        _inputManager->ResetMouseMove();
        _inputManager->UpdateTimerActions(frameTimestep);

        window.FetchEvents();

        _inputManager->PropagateActionEvents();
        _frameListenerManager->_DispatchQueuedEventsToFrameListeners();
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_IconifiedSleep() KMP_PROFILING(ProfileLevelMinor)
    {
        const auto iconifiedSleepTimeMs = 1000.0f / float(_iconifiedFPS);
        const auto elapsedTimeThisFrame = _frameClock.Peek();

        if (elapsedTimeThisFrame < iconifiedSleepTimeMs)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(UInt32(iconifiedSleepTimeMs - elapsedTimeThisFrame)));
        }
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_SaveSettings() const KMP_PROFILING(ProfileLevelImportant)
    {
        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

        settings.value().get().SaveUInt(IconifiedFPSStr, _iconifiedFPS);
        
        _windowBackend->SaveSettings(*settings);
        _graphicsBackend->SaveSettings(*settings);
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_LoadWindowBackendSettings(SettingsDocument& settingsDocument) KMP_PROFILING(ProfileLevelImportant)
    {
        _windowBackend->LoadSettings(settingsDocument);
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_LoadGraphicsBackendSettings(SettingsDocument& settingsDocument) KMP_PROFILING(ProfileLevelImportant)
    {
        _graphicsBackend->LoadSettings(settingsDocument);
    }}
    //--------------------------------------------------------------------------

    void WindowApplication::_LoadSettings(SettingsDocument& settingsDocument) KMP_PROFILING(ProfileLevelImportant)
    {
        _iconifiedFPS = settingsDocument.GetUInt(IconifiedFPSStr, IconifiedFPSMin);
        if (_iconifiedFPS < IconifiedFPSMin || _iconifiedFPS > IconifiedFPSMax)
        {
            KMP_LOG_WARN("iconified FPS ({} from settings) required to be in range from 10 to 60, value will be clamped", _iconifiedFPS);
            _iconifiedFPS = Math::Clamp(_iconifiedFPS, IconifiedFPSMin, IconifiedFPSMax);
        }
    }}
    //--------------------------------------------------------------------------
}
