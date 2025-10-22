#include "Application/editor_application.h"

#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "EditorApplication";

    EditorApplication::EditorApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("EditorApplication::EditorApplication(const WindowApplicationParameters&)")
        , _mainWindow(_windowBackend->GetMainWindow())
        , _uiFrameListener(nullptr)
        , _systemMetricsFrameListener(nullptr)
    {
        Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Initialize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        _mainWindow.SetTitle(_applicationName.c_str());

        if (!_assetsManager->LoadAssetFile("editor_assets.kmpdata"))
        {
            KMP_LOG_ERROR("failed to load editor assets");
        }

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_EDITOR);

        _systemMetricsFrameListener.reset(new SystemMetricsFrameListener(*_systemMetricsManager));

        _uiFrameListener.reset(new EditorFrameListener(_mainWindow, *_graphicsBackend, *_assetsManager, *_localizationManager, *_systemMetricsManager, _systemMetricsFrameListener->GetTimer()));

        LoadSettings();

        AddFrameListener(_systemMetricsFrameListener.get());
        AddFrameListener(_uiFrameListener.get());
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelAlways);

        SaveSettings();

        _uiFrameListener.reset();
        _systemMetricsFrameListener.reset();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings() const
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

        _systemMetricsFrameListener->SaveSettings(*settings);
        _uiFrameListener->SaveSettings(*settings);
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings()
    {
        KMP_PROFILE_FUNCTION(ProfileLevelImportantFunctions);

        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get setting entry for loading");
            return;
        }

        _systemMetricsFrameListener->LoadSettings(*settings);
        _uiFrameListener->LoadSettings(*settings);
    }
    //--------------------------------------------------------------------------
}