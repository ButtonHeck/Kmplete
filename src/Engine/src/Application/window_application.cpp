#include "Kmplete/Application/window_application.h"
#include "Kmplete/Core/window.h"
#include "Kmplete/Core/assertion.h"
#include "Kmplete/Log/log.h"

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

    void WindowApplication::SaveSettings(const Path& path /*= Path()*/) const
    {
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();
        Application::SaveSettings(path);
    }
    //--------------------------------------------------------------------------

    void WindowApplication::LoadSettings(const Path& path /*= Path()*/)
    {
        KMP_PROFILE_FUNCTION();

        Application::LoadSettings(path);
        LoadSettingsInternal();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::Initialize()
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

        SaveSettingsInternal();

        _graphicsBackend.reset();
        _windowBackend.reset();
    }
    //--------------------------------------------------------------------------

    void WindowApplication::SaveSettingsInternal() const
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

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