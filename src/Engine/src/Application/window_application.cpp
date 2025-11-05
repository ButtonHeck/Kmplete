#include "Kmplete/Application/window_application.h"
#include "Kmplete/Internal/default_window_icon_data.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Math/math.h"
#include "Kmplete/Filesystem/filesystem.h"


#if defined (CreateWindow)
    #pragma push_macro("CreateWindow")
    #undef CreateWindow
    #define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    static constexpr auto SettingsEntryName = "WindowApplication";
    static constexpr auto GraphicsBackendTypeStr = "GraphicsBackendType";

    WindowApplication::WindowApplication(const WindowApplicationParameters& parameters)
        : Application(parameters.applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _windowBackend(nullptr)
        , _graphicsBackend(nullptr)
        , _assetsManager(nullptr)
        , _frameListenerManager(nullptr)
        , _graphicsBackendType(GraphicsBackendType::OpenGL)
        , _frameTimer(0)
    {
        Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Run()
    {
        KMP_LOG_INFO_FN("'{}' main loop started...", _applicationName);

        _running = true;

        auto& mainWindow = _windowBackend->GetMainWindow();

        while (_running)
        {
            if (!RunFrameIteration(mainWindow))
            {
                break;
            }
        }

        KMP_LOG_INFO_FN("'{}' main loop finished", _applicationName);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::AddFrameListener(NonNull<FrameListener*> frameListener)
    {
        _frameListenerManager->AddFrameListener(frameListener);
    }
    //--------------------------------------------------------------------------

    bool WindowApplication::ConfirmExit()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::OnEvent(Event& event)
    {
        _frameListenerManager->ProcessEventsFrameListeners(event);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize(const WindowApplicationParameters& parameters)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);

        LoadSettings();

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(defaultWindowIcon);
        mainWindow.SetResizable(parameters.resizable);
        mainWindow.SetEventCallback(KMP_BIND(WindowApplication::OnEvent));

        _graphicsBackend = GraphicsBackend::Create(_graphicsBackendType);
        KMP_ASSERT(_graphicsBackend);

        _assetsManager = CreateUPtr<Assets::AssetsManager>(_applicationPath, _graphicsBackend->GetType());
        KMP_ASSERT(_assetsManager);

        _frameListenerManager = CreateUPtr<FrameListenerManager>();
        KMP_ASSERT(_frameListenerManager);

        _frameTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        SaveSettings();

        _frameListenerManager.reset();
        _assetsManager.reset();
        _graphicsBackend.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    bool WindowApplication::RunFrameIteration(Window& window)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        const auto frameTimestep = Math::Clamp(_frameTimer.Mark(), 0.0f, 100.0f);

        window.ProcessEvents();

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

        _frameListenerManager->UpdateFrameListeners(frameTimestep, mainWindowIsIconified);

        if (!mainWindowIsIconified)
        {
            _frameListenerManager->RenderFrameListeners();
        }

        _frameListenerManager->ProcessFrameListenersCommands();

        window.SwapBuffers();

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
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

    void WindowApplication::LoadSettings()
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

#if defined (KMP_UNDEF_CreateWindow)
    #pragma pop_macro("CreateWindow")
    #undef KMP_UNDEF_CreateWindow
#endif