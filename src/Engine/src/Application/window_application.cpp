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
    constexpr static auto GraphicsBackendTypeStr = "GraphicsBackendType";

    WindowApplication::WindowApplication(const ApplicationParameters& applicationParameters)
        : Application(applicationParameters)
        , _windowBackend(nullptr)
        , _graphicsBackendType(GraphicsBackendType::OpenGL)
        , _graphicsBackend(nullptr)
    {
        Initialize();
    }
    //--------------------------------------------------------------------------

    WindowApplication::~WindowApplication()
    {
        Finalize();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettings(const Path& path /*= Path()*/) const
    {
        SaveSettingsInternal();
        Application::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings(const Path& path /*= Path()*/)
    {
        Application::LoadSettings(path);
        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        _windowBackend = WindowBackend::Create();
        KMP_ASSERT(_windowBackend);

        LoadSettingsInternal();

        auto& mainWindow = _windowBackend->CreateMainWindow();
        mainWindow.SetIcon(KMP_DEFAULT_WINDOW_ICON_PATH);

        _graphicsBackend = GraphicsBackend::Create(_graphicsBackendType);
        KMP_ASSERT(_graphicsBackend);

        if (!_graphicsBackend->GetTextureManager().CreateTexture(0, Utils::Concatenate(KMP_ICONS_FOLDER, "error.jpg")))
        {
            KMP_LOG_CRITICAL("WindowApplication: error texture loading failed");
            throw std::runtime_error("WindowApplication: error texture loading failed");
        }
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Finalize()
    {
        SaveSettingsInternal();

        _graphicsBackend.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettingsInternal() const
    {
        auto settings = _settingsManager->PutSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("WindowApplication: failed to create settings entry for saving");
            return;
        }
        
        settings->SaveString(GraphicsBackendTypeStr, GraphicsBackendTypeToString(_graphicsBackendType));
        _windowBackend->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettingsInternal()
    {
        const auto settings = _settingsManager->GetSettings(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("WindowApplication: failed to get setting entry for loading");
            return;
        }

        _graphicsBackendType = StringToGraphicsBackendType(settings->GetString(GraphicsBackendTypeStr, DefaultAPIStr));
        _windowBackend->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}

#if defined (KMP_UNDEF_CreateWindow)
    #pragma pop_macro("CreateWindow")
    #undef KMP_UNDEF_CreateWindow
#endif