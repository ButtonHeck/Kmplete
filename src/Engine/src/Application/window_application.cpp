#include "Kmplete/Application/window_application.h"
#include "Kmplete/Window/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"
#include "Kmplete/Utils/function_utils.h"

#include <stdexcept>

#if defined (CreateWindow)
    #pragma push_macro("CreateWindow")
    #undef CreateWindow
    #define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "WindowApplication";
    constexpr static auto GraphicsBackendTypeStr = "GraphicsBackendType";

    WindowApplication::WindowApplication(const ApplicationParameters& applicationParameters)
        : Application(applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("WindowApplication::WindowApplication(const ApplicationParameters&)")
        , _windowBackend(nullptr)
        , _graphicsBackendType(GraphicsBackendType::OpenGL)
        , _graphicsBackend(nullptr)
    {
        Initialize();

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

            for (auto& frameListener : _frameListeners)
            {
                frameListener->Update(0.0f, mainWindowIsIconified);
            }

            if (!mainWindowIsIconified)
            {
                for (auto& frameListener : _frameListeners)
                {
                    frameListener->Render();
                }
            }

            mainWindow.SwapBuffers();
        }

        KMP_LOG_INFO_FN("'{}' main loop finished", _applicationName);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::OnEvent(Event& event)
    {
        for (auto iter = _frameListeners.rbegin(); iter != _frameListeners.rend(); ++iter)
        {
            if (event.handled)
            {
                break;
            }

            (*iter)->OnEvent(event);
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);

        LoadSettings();

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(KMP_DEFAULT_WINDOW_ICON_PATH);

        _graphicsBackend = GraphicsBackend::Create(_graphicsBackendType);
        KMP_ASSERT(_graphicsBackend);

        if (!_graphicsBackend->GetTextureManager().CreateTexture(0, Utils::Concatenate(KMP_ICONS_FOLDER, "error.jpg")))
        {
            KMP_LOG_CRITICAL("error texture loading failed");
            throw std::runtime_error("WindowApplication: error texture loading failed");
        }

        mainWindow.SetEventCallback(KMP_BIND(WindowApplication::OnEvent));
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettings();

        _graphicsBackend.reset();
        _windowBackend.reset();
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