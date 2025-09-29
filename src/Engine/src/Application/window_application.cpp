#include "Kmplete/Application/window_application.h"
#include "Kmplete/Internal/default_window_icon_data.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Utils/function_utils.h"
#include "Kmplete/Utils/string_utils.h"
#include "Kmplete/Math/math.h"


#if defined (CreateWindow)
    #pragma push_macro("CreateWindow")
    #undef CreateWindow
    #define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "WindowApplication";
    constexpr static auto GraphicsBackendTypeStr = "GraphicsBackendType";

    WindowApplication::WindowApplication(const WindowApplicationParameters& parameters)
        : Application(parameters.applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("WindowApplication::WindowApplication(const WindowApplicationParameters&)")
        , _windowBackend(nullptr)
        , _graphicsBackendType(GraphicsBackendType::OpenGL)
        , _graphicsBackend(nullptr)
        , _frameTimer(0)
    {
        Initialize(parameters);

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        KMP_PROFILE_FUNCTION();

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
        _frameListeners.push_back(frameListener);
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
        KMP_PROFILE_FUNCTION();

        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);

        LoadSettings();

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(defaultWindowIcon);
        mainWindow.SetResizable(parameters.resizable);

        _graphicsBackend = GraphicsBackend::Create(_graphicsBackendType);
        KMP_ASSERT(_graphicsBackend);

        mainWindow.SetEventCallback(KMP_BIND(WindowApplication::OnEvent));

        _frameTimer.Mark();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettings();

        _frameListeners.clear();

        _graphicsBackend.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::UpdateFrameListeners(float frameTimestep, bool mainWindowIsIconified)
    {
        for (auto& frameListener : _frameListeners)
        {
            if (frameListener->IsActive())
            {
                frameListener->Update(frameTimestep, mainWindowIsIconified);
            }
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::RenderFrameListeners()
    {
        for (auto& frameListener : _frameListeners)
        {
            if (frameListener->IsActive())
            {
                frameListener->Render();
            }
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::ProcessEventsFrameListeners(Event& event)
    {
        for (auto iter = _frameListeners.rbegin(); iter != _frameListeners.rend(); ++iter)
        {
            auto frameListener = *iter;
            if (frameListener->IsActive())
            {
                if (event.handled)
                {
                    break;
                }

                frameListener->OnEvent(event);
            }
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings() const
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

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