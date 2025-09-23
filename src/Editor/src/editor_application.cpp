#include "editor_application.h"
#include "Kmplete/Core/settings_document.h"
#include "Kmplete/Log/log.h"

namespace Kmplete
{
    constexpr static auto SettingsEntryName = "EditorApplication";

    EditorApplication::EditorApplication(const ApplicationParameters& applicationParameters)
        : WindowApplication(applicationParameters)
          KMP_PROFILE_CONSTRUCTOR_START_DERIVED_CLASS("EditorApplication::EditorApplication(const ApplicationParameters&)")
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
        KMP_PROFILE_FUNCTION();

        Finalize();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Initialize()
    {
        KMP_PROFILE_FUNCTION();

        _mainWindow.SetTitle(_applicationName.c_str());

        _graphicsBackend->GetTextureManager().CreateTexture("_flag_russian"_sid, Utils::Concatenate(KMP_ICONS_FOLDER, "flag_russia_128.png"));
        _graphicsBackend->GetTextureManager().CreateTexture("_flag_usa"_sid, Utils::Concatenate(KMP_ICONS_FOLDER, "flag_usa_128.png"));

        _localizationManager->AddMessagesDomain(KMP_TR_DOMAIN_EDITOR);

        _systemMetricsFrameListener.reset(new SystemMetricsFrameListener(*_systemMetricsManager));

        _uiFrameListener.reset(new EditorFrameListener(_mainWindow, *_graphicsBackend, *_localizationManager, *_systemMetricsManager, _systemMetricsFrameListener->GetTimer()));

        LoadSettings();

        AddFrameListener(_systemMetricsFrameListener.get());
        AddFrameListener(_uiFrameListener.get());
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Finalize()
    {
        KMP_PROFILE_FUNCTION();

        SaveSettings();

        _uiFrameListener.reset();
        _systemMetricsFrameListener.reset();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings() const
    {
        KMP_PROFILE_FUNCTION();

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
        KMP_PROFILE_FUNCTION();

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