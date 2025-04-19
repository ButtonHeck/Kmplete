#include "Kmplete/Application/window_application.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Core/log.h"

#include <stdexcept>

#if defined (CreateWindow)
    #pragma push_macro("CreateWindow")
    #undef CreateWindow
    #define KMP_UNDEF_CreateWindow
#endif

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "WindowApplication";

    WindowApplication::WindowApplication(const ApplicationParameters& applicationParameters)
        : Application(applicationParameters)
        , _windowBackend(nullptr)
        , _graphicsManager(nullptr)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings(const Path& path) const
    {
        SaveSettingsInternal();
        Application::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings(const Path& path)
    {
        Application::LoadSettings(path);
        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);

        _graphicsManager = CreateUPtr<GraphicsManager>();
        KMP_ASSERT(_graphicsManager);

        LoadSettingsInternal();

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(KMP_DEFAULT_WINDOW_ICON_PATH);

        if (!_graphicsManager->CreateBackend())
        {
            KMP_LOG_CORE_CRITICAL("WindowApplication: graphics backend initialization failed");
            throw std::runtime_error("WindowApplication: graphics backend initialization failed");
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        SaveSettingsInternal();

        _graphicsManager.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettingsInternal() const
    {
        auto settings = _settingsManager->PutSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_CORE_WARN("WindowApplication: failed to create settings entry for saving");
            return;
        }
        
        _windowBackend->SaveSettings(*settings);
        _graphicsManager->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettingsInternal()
    {
        const auto settings = _settingsManager->GetSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_CORE_WARN("WindowApplication: failed to get setting entry for loading");
            return;
        }

        _windowBackend->LoadSettings(*settings);
        _graphicsManager->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_CreateWindow)
    #pragma pop_macro("CreateWindow")
    #undef KMP_UNDEF_CreateWindow
#endif