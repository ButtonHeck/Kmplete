#include "Application/editor_application.h"

#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Log/log.h"


namespace Kmplete
{
    static constexpr auto SettingsEntryName = "EditorApplication";


    EditorApplication::EditorApplication(const WindowApplicationParameters& parameters)
        : WindowApplication(parameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS()
        , _mainWindow(_windowBackend->GetMainWindow())
        , _uiFrameListener(nullptr)
    {
        _Initialize();

        KMP_PROFILE_CONSTRUCTOR_END()
    }
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication() KMP_PROFILING(ProfileLevelAlways)
    {
        _Finalize();
    }}
    //--------------------------------------------------------------------------

    void EditorApplication::_Initialize() KMP_PROFILING(ProfileLevelAlways)
    {
        _mainWindow.SetTitle(_applicationName.c_str());

        if (!_assetsManager->LoadAssetFile("editor_assets.kmpdata"))
        {
            KMP_LOG_ERROR("failed to load editor assets");
        }

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_EDITOR);

        _uiFrameListener.reset(new EditorFrameListener(*_frameListenerManager.get(), _mainWindow, *_graphicsBackend, *_assetsManager, *_localizationManager, *_systemMetricsManager, *_inputManager));

        _LoadSettings();
    }}
    //--------------------------------------------------------------------------

    void EditorApplication::_Finalize() KMP_PROFILING(ProfileLevelAlways)
    {
        _SaveSettings();

        _uiFrameListener.reset();
    }}
    //--------------------------------------------------------------------------

    void EditorApplication::_SaveSettings() const KMP_PROFILING(ProfileLevelImportant)
    {
        auto settings = _settingsManager->PutSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to create settings entry for saving");
            return;
        }

        _uiFrameListener->SaveSettings(*settings);
    }}
    //--------------------------------------------------------------------------

    void EditorApplication::_LoadSettings() KMP_PROFILING(ProfileLevelImportant)
    {
        const auto settings = _settingsManager->GetSettingsDocument(SettingsEntryName);
        if (!settings)
        {
            KMP_LOG_WARN("failed to get setting entry for loading");
            return;
        }

        _uiFrameListener->LoadSettings(*settings);
    }}
    //--------------------------------------------------------------------------
}