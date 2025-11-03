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
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("WindowApplication::WindowApplication(const WindowApplicationParameters&)")
        , _windowBackend(nullptr)
        , _graphicsBackend(nullptr)
        , _assetsManager(nullptr)
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
            KMP_PROFILE_SCOPE("Frame iteration", ProfileLevelAlways);

            const auto frameTimestep = Math::Clamp(_frameTimer.Mark(), 0.0f, 100.0f);

            mainWindow.ProcessEvents();

            if (mainWindow.ShouldClose())
            {
                if (ConfirmExit())
                {
                    _running = false;
                    break;
                }
                else
                {
                    mainWindow.SetShouldClose(false);
                }
            }

            const auto mainWindowIsIconified = mainWindow.IsIconified();

            UpdateFrameListeners(frameTimestep, mainWindowIsIconified);

            if (!mainWindowIsIconified)
            {
                RenderFrameListeners();
            }

            mainWindow.SwapBuffers();
        }

        KMP_LOG_INFO_FN("'{}' main loop finished", _applicationName);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::AddFrameListener(NonNull<FrameListener*> frameListener)
    {
        _frameListenersWrappers.emplace_back(FrameListenerWrapper(frameListener, true));
    }
    //--------------------------------------------------------------------------

    bool WindowApplication::ConfirmExit()
    {
        return true;
    }
    //--------------------------------------------------------------------------

    void WindowApplication::OnEvent(Event& event)
    {
        ProcessEventsFrameListeners(event);
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

        _frameTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        SaveSettings();

        _frameListenersWrappers.clear();

        _assetsManager.reset();
        _graphicsBackend.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified)
    {
        for (auto& wrapper : _frameListenersWrappers)
        {
            if (wrapper.isActive)
            {
                wrapper.frameListener->Update(frameTimestep, mainWindowIsIconified);
            }
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::RenderFrameListeners()
    {
        for (auto& wrapper : _frameListenersWrappers)
        {
            if (wrapper.isActive)
            {
                wrapper.frameListener->Render();
            }
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::ProcessEventsFrameListeners(Event& event)
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        for (auto iter = _frameListenersWrappers.rbegin(); iter != _frameListenersWrappers.rend(); ++iter)
        {
            auto wrapper = *iter;
            if (wrapper.isActive)
            {
                if (event.handled)
                {
                    break;
                }

                wrapper.frameListener->OnEvent(event);
            }
        }
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